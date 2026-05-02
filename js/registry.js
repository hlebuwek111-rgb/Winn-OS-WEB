const STORAGE_KEY = 'winn_os_registry';

const state = {
  users: [
    { id: 'admin', displayName: 'Administrator', password: 'admin123' },
    { id: 'guest', displayName: 'Guest', password: 'guest' },
  ],
  session: {
    authenticated: false,
    currentUserId: null,
    startedAt: null,
  },
};

function safeLoad() {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) return;
    const parsed = JSON.parse(raw);
    if (parsed && typeof parsed === 'object') {
      Object.assign(state, parsed);
    }
  } catch (_err) {
    // Ignore corrupt storage; defaults will be used.
  }
}

function save() {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
}

safeLoad();

export function getUsers() {
  return [...state.users];
}

export function findUser(userId) {
  return state.users.find((user) => user.id === userId) || null;
}

export function validatePassword(userId, password) {
  const user = findUser(userId);
  return Boolean(user && user.password === password);
}

export function getSession() {
  return { ...state.session };
}

export function setSession(partialSession) {
  state.session = { ...state.session, ...partialSession };
  save();
  return getSession();
}

export function clearSession() {
  return setSession({
    authenticated: false,
    currentUserId: null,
    startedAt: null,
  });
}
