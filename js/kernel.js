import { runBiosStage } from './bios.js';
import { runBootloaderStage } from './bootloader.js';
import { runLoginStage } from './login.js';
import { mountDesktop } from './desktop.js';
import { mountTaskbar } from './taskbar.js';
import { unlockOnFirstInteraction, playStartupChime } from './soundManager.js';
import { findUser, setSession, clearSession } from './registry.js';

function createEventBus() {
  const listeners = new Map();

  function on(eventName, callback, options = {}) {
    const entry = { callback, once: Boolean(options.once) };
    if (!listeners.has(eventName)) listeners.set(eventName, []);
    listeners.get(eventName).push(entry);

    return () => {
      const queue = listeners.get(eventName) || [];
      listeners.set(
        eventName,
        queue.filter((item) => item !== entry),
      );
    };
  }

  function emit(eventName, payload) {
    const queue = [...(listeners.get(eventName) || [])];
    queue.forEach((entry) => {
      entry.callback(payload);
      if (entry.once) {
        const current = listeners.get(eventName) || [];
        listeners.set(
          eventName,
          current.filter((item) => item !== entry),
        );
      }
    });
  }

  return { on, emit };
}

export function bootKernel(root = document.getElementById('app')) {
  if (!root) throw new Error('Kernel root element not found.');

  const eventBus = createEventBus();
  clearSession();

  unlockOnFirstInteraction(eventBus);

  eventBus.on('bios:complete', () => {
    runBootloaderStage(eventBus, root);
  });

  eventBus.on('audio:unlocked', () => {
    playStartupChime(eventBus);
  });

  eventBus.on('bootloader:readyForLogin', () => {
    runLoginStage(eventBus, root);
  });

  eventBus.on('auth:success', ({ userId, authenticatedAt }) => {
    const session = setSession({
      authenticated: true,
      currentUserId: userId,
      startedAt: authenticatedAt,
    });
    const currentUser = findUser(session.currentUserId);
    if (!currentUser) return;

    mountDesktop(root, currentUser);
    mountTaskbar(root, currentUser);
    eventBus.emit('desktop:mounted', { userId: currentUser.id });
  });

  runBiosStage(eventBus, root);

  return eventBus;
}

bootKernel();
