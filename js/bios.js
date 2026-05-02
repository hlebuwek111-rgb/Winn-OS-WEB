const BIOS_DELAY_MS = 3000;

export function runBiosStage(eventBus, root) {
  root.innerHTML = `
    <section class="bios-screen">
      <h1>WINN BIOS v1.0</h1>
      <p>Memory Check.............OK</p>
      <p>Storage Check............OK</p>
      <p>Press <strong>F2</strong> for Setup</p>
    </section>
    <dialog id="bios-setup-modal">
      <h2>BIOS Setup</h2>
      <p>Boot Order: WEB &gt; SAFE MODE</p>
      <button id="bios-setup-close">Close</button>
    </dialog>
  `;

  const modal = root.querySelector('#bios-setup-modal');
  const closeButton = root.querySelector('#bios-setup-close');

  const onKeyDown = (event) => {
    if (event.key === 'F2') {
      event.preventDefault();
      if (modal?.showModal) modal.showModal();
    }
  };

  closeButton?.addEventListener('click', () => modal?.close());
  document.addEventListener('keydown', onKeyDown);

  setTimeout(() => {
    document.removeEventListener('keydown', onKeyDown);
    eventBus.emit('bios:complete');
  }, BIOS_DELAY_MS);
}
