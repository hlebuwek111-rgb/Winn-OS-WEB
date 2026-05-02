export function mountTaskbar(root, currentUser) {
  const taskbar = document.createElement('footer');
  taskbar.className = 'taskbar';
  taskbar.innerHTML = `
    <button class="start-button">Start</button>
    <span class="taskbar-user">${currentUser.displayName}</span>
  `;
  root.appendChild(taskbar);
}
