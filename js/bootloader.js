export function runBootloaderStage(eventBus, root) {
  root.innerHTML = `
    <section class="bootloader-screen">
      <div class="boot-logo">Winn OS</div>
      <div class="spinner" aria-label="Loading"></div>
      <p>Loading core services...</p>
      <small>Click or press any key to enable startup audio.</small>
    </section>
  `;

  const onUnlocked = () => {
    eventBus.emit('bootloader:readyForLogin');
  };

  eventBus.on('audio:startupChimePlayed', onUnlocked, { once: true });
  eventBus.on('audio:chimeSkipped', onUnlocked, { once: true });
}
