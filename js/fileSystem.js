class FileSystem {
  constructor() {
    this.dbName = 'WinnFS';
    this.dbVersion = 1;
    this.db = null;
  }

  async init() {
    if (this.db) return this.db;

    this.db = await new Promise((resolve, reject) => {
      const request = indexedDB.open(this.dbName, this.dbVersion);

      request.onupgradeneeded = (event) => {
        const db = event.target.result;

        if (!db.objectStoreNames.contains('items')) {
          const items = db.createObjectStore('items', { keyPath: 'id' });
          items.createIndex('parentId', 'parentId', { unique: false });
          items.createIndex('name', 'name', { unique: false });
        }

        if (!db.objectStoreNames.contains('recycle')) {
          db.createObjectStore('recycle', { keyPath: 'id' });
        }
      };

      request.onsuccess = () => resolve(request.result);
      request.onerror = () => reject(request.error);
    });

    const count = await this.#countItems();
    if (count === 0) {
      await this.#seedInitialStructure();
    }

    return this.db;
  }

  async getItems(parentId = null) {
    await this.init();
    const normalizedParentId = parentId == null ? null : this.#normalizePath(parentId);

    return this.#request('items', 'readonly', (store) => {
      const index = store.index('parentId');
      return index.getAll(IDBKeyRange.only(normalizedParentId));
    });
  }

  async getItem(pathOrId) {
    await this.init();
    const normalized = this.#normalizePath(pathOrId);
    return this.#request('items', 'readonly', (store) => store.get(normalized));
  }

  async createFile(path, content = '') {
    await this.init();
    const normalized = this.#normalizePath(path);
    const { parentId, name } = this.#splitPath(normalized);

    await this.#ensureParentExists(parentId);
    await this.#throwIfExists(normalized);

    const now = new Date().toISOString();
    const item = {
      id: normalized,
      parentId,
      name,
      type: 'file',
      content,
      createdAt: now,
      updatedAt: now
    };

    await this.#request('items', 'readwrite', (store) => store.add(item));
    return item;
  }

  async createDirectory(path) {
    await this.init();
    const normalized = this.#normalizePath(path);
    const { parentId, name } = this.#splitPath(normalized);

    await this.#ensureParentExists(parentId);
    await this.#throwIfExists(normalized);

    const now = new Date().toISOString();
    const item = {
      id: normalized,
      parentId,
      name,
      type: 'directory',
      createdAt: now,
      updatedAt: now
    };

    await this.#request('items', 'readwrite', (store) => store.add(item));
    return item;
  }

  async readFile(path) {
    const item = await this.getItem(path);
    if (!item || item.type !== 'file') return null;
    return item.content ?? '';
  }

  async writeFile(path, content) {
    await this.init();
    const existing = await this.getItem(path);

    if (!existing) {
      return this.createFile(path, content);
    }

    if (existing.type !== 'file') {
      throw new Error('Cannot write to a directory.');
    }

    existing.content = content;
    existing.updatedAt = new Date().toISOString();
    await this.#request('items', 'readwrite', (store) => store.put(existing));
    return existing;
  }

  async deleteItem(pathOrId) {
    await this.init();
    const id = this.#normalizePath(pathOrId);
    const item = await this.getItem(id);
    if (!item) return false;

    const descendants = item.type === 'directory' ? await this.#collectDescendants(id) : [];
    const all = [item, ...descendants];

    await this.#transaction(['items', 'recycle'], 'readwrite', async ({ items, recycle }) => {
      for (const entry of all) {
        await this.#requestFromStore(recycle, () => recycle.put({ ...entry, deletedAt: new Date().toISOString() }));
        await this.#requestFromStore(items, () => items.delete(entry.id));
      }
    });

    return true;
  }

  async restoreItem(pathOrId) {
    await this.init();
    const id = this.#normalizePath(pathOrId);
    const recycleItem = await this.#request('recycle', 'readonly', (store) => store.get(id));
    if (!recycleItem) return false;

    const descendants = recycleItem.type === 'directory' ? await this.#collectDescendants(id, 'recycle') : [];
    const all = [recycleItem, ...descendants];

    for (const entry of all) {
      if (entry.parentId && !(await this.getItem(entry.parentId))) {
        throw new Error(`Cannot restore ${entry.id}; missing parent ${entry.parentId}.`);
      }
    }

    await this.#transaction(['items', 'recycle'], 'readwrite', async ({ items, recycle }) => {
      for (const entry of all) {
        const { deletedAt, ...restored } = entry;
        restored.updatedAt = new Date().toISOString();
        await this.#requestFromStore(items, () => items.put(restored));
        await this.#requestFromStore(recycle, () => recycle.delete(entry.id));
      }
    });

    return true;
  }

  async emptyRecycle() {
    await this.init();
    await this.#request('recycle', 'readwrite', (store) => store.clear());
    return true;
  }

  async rename(pathOrId, newName) {
    await this.init();
    const id = this.#normalizePath(pathOrId);
    const item = await this.getItem(id);
    if (!item) throw new Error('Item not found.');

    const parentId = item.parentId;
    const newId = parentId ? `${parentId}/${newName}` : newName;
    return this.move(id, newId);
  }

  async move(sourcePathOrId, destinationPathOrId) {
    await this.init();
    const sourceId = this.#normalizePath(sourcePathOrId);
    const destinationId = this.#normalizePath(destinationPathOrId);

    const item = await this.getItem(sourceId);
    if (!item) throw new Error('Source item not found.');
    await this.#throwIfExists(destinationId);

    const { parentId: newParentId, name: newName } = this.#splitPath(destinationId);
    await this.#ensureParentExists(newParentId);

    const descendants = item.type === 'directory' ? await this.#collectDescendants(sourceId) : [];

    await this.#request('items', 'readwrite', async (store) => {
      const now = new Date().toISOString();
      const updates = [item, ...descendants].map((entry) => {
        const suffix = entry.id.slice(sourceId.length);
        const newId = `${destinationId}${suffix}`;
        const updated = { ...entry, id: newId, updatedAt: now };
        if (entry.id === sourceId) {
          updated.parentId = newParentId;
          updated.name = newName;
        } else {
          const relParent = entry.parentId.slice(sourceId.length);
          updated.parentId = `${destinationId}${relParent}`;
        }
        return updated;
      });

      for (const entry of [item, ...descendants]) {
        await this.#requestFromStore(store, () => store.delete(entry.id));
      }
      for (const entry of updates) {
        await this.#requestFromStore(store, () => store.put(entry));
      }
    });

    return this.getItem(destinationId);
  }

  async copy(sourcePathOrId, destinationPathOrId) {
    await this.init();
    const sourceId = this.#normalizePath(sourcePathOrId);
    const destinationId = this.#normalizePath(destinationPathOrId);

    const item = await this.getItem(sourceId);
    if (!item) throw new Error('Source item not found.');
    await this.#throwIfExists(destinationId);

    const { parentId: newParentId, name: newName } = this.#splitPath(destinationId);
    await this.#ensureParentExists(newParentId);

    const descendants = item.type === 'directory' ? await this.#collectDescendants(sourceId) : [];
    const now = new Date().toISOString();

    for (const entry of [item, ...descendants]) {
      const suffix = entry.id.slice(sourceId.length);
      const newId = `${destinationId}${suffix}`;
      const copied = { ...entry, id: newId, createdAt: now, updatedAt: now };
      if (entry.id === sourceId) {
        copied.parentId = newParentId;
        copied.name = newName;
      } else {
        const relParent = entry.parentId.slice(sourceId.length);
        copied.parentId = `${destinationId}${relParent}`;
      }
      await this.#request('items', 'readwrite', (store) => store.add(copied));
    }

    return this.getItem(destinationId);
  }

  async pathExists(path) {
    return Boolean(await this.getItem(path));
  }

  #normalizePath(path) {
    if (path == null) return null;
    let normalized = String(path).trim().replace(/\\+/g, '/');
    normalized = normalized.replace(/\/+/g, '/');
    if (normalized.length > 3) normalized = normalized.replace(/\/$/, '');
    return normalized;
  }

  #splitPath(normalizedPath) {
    const idx = normalizedPath.lastIndexOf('/');
    if (idx === -1) return { parentId: null, name: normalizedPath };
    return {
      parentId: normalizedPath.slice(0, idx),
      name: normalizedPath.slice(idx + 1)
    };
  }

  async #seedInitialStructure() {
    const seedItems = [
      { id: 'C:', parentId: null, name: 'C:', type: 'directory' },
      { id: 'D:', parentId: null, name: 'D:', type: 'directory' },

      { id: 'C:/Windows', parentId: 'C:', name: 'Windows', type: 'directory' },
      { id: 'C:/Program Files', parentId: 'C:', name: 'Program Files', type: 'directory' },
      { id: 'C:/Users', parentId: 'C:', name: 'Users', type: 'directory' },
      { id: 'D:/Data', parentId: 'D:', name: 'Data', type: 'directory' },

      { id: 'C:/Users/Guest', parentId: 'C:/Users', name: 'Guest', type: 'directory' },
      { id: 'C:/Users/Admin', parentId: 'C:/Users', name: 'Admin', type: 'directory' },

      { id: 'C:/Users/Guest/Desktop', parentId: 'C:/Users/Guest', name: 'Desktop', type: 'directory' },
      { id: 'C:/Users/Guest/Documents', parentId: 'C:/Users/Guest', name: 'Documents', type: 'directory' },
      { id: 'C:/Users/Admin/Desktop', parentId: 'C:/Users/Admin', name: 'Desktop', type: 'directory' },
      { id: 'C:/Users/Admin/Documents', parentId: 'C:/Users/Admin', name: 'Documents', type: 'directory' },

      {
        id: 'C:/Users/Guest/Desktop/Welcome.txt',
        parentId: 'C:/Users/Guest/Desktop',
        name: 'Welcome.txt',
        type: 'file',
        content: 'Welcome to Winn OS!'
      },
      {
        id: 'C:/Users/Guest/Documents/Notes.txt',
        parentId: 'C:/Users/Guest/Documents',
        name: 'Notes.txt',
        type: 'file',
        content: 'This is a sample note file.'
      }
    ];

    const now = new Date().toISOString();
    await this.#request('items', 'readwrite', async (store) => {
      for (const item of seedItems) {
        await this.#requestFromStore(store, () =>
          store.add({ ...item, createdAt: now, updatedAt: now })
        );
      }
    });
  }

  async #collectDescendants(rootId, storeName = 'items') {
    const all = await this.#request(storeName, 'readonly', (store) => store.getAll());
    return all.filter((item) => item.id !== rootId && item.id.startsWith(`${rootId}/`));
  }

  async #ensureParentExists(parentId) {
    if (parentId == null) return;
    const parent = await this.getItem(parentId);
    if (!parent || parent.type !== 'directory') {
      throw new Error(`Parent directory does not exist: ${parentId}`);
    }
  }

  async #throwIfExists(id) {
    const existing = await this.getItem(id);
    if (existing) throw new Error(`Path already exists: ${id}`);
  }

  async #countItems() {
    return this.#request('items', 'readonly', (store) => store.count());
  }

  #request(storeName, mode, handler) {
    return new Promise((resolve, reject) => {
      const tx = this.db.transaction(storeName, mode);
      const store = tx.objectStore(storeName);

      Promise.resolve(handler(store))
        .then((request) => {
          if (!request || typeof request.onsuccess !== 'object' && typeof request.onsuccess !== 'function') {
            tx.oncomplete = () => resolve(request);
            tx.onerror = () => reject(tx.error);
            return;
          }

          request.onsuccess = () => resolve(request.result);
          request.onerror = () => reject(request.error);
        })
        .catch(reject);
    });
  }

  #requestFromStore(store, makeRequest) {
    return new Promise((resolve, reject) => {
      const request = makeRequest();
      request.onsuccess = () => resolve(request.result);
      request.onerror = () => reject(request.error);
    });
  }

  async #transaction(storeNames, mode, handler) {
    const tx = this.db.transaction(storeNames, mode);
    const stores = Object.fromEntries(storeNames.map((name) => [name, tx.objectStore(name)]));
    await handler(stores);
    await new Promise((resolve, reject) => {
      tx.oncomplete = resolve;
      tx.onerror = () => reject(tx.error);
      tx.onabort = () => reject(tx.error);
    });
  }
}

if (typeof module !== 'undefined' && module.exports) {
  module.exports = FileSystem;
}

export default FileSystem;
