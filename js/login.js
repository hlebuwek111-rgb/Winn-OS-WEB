import { getUsers, validatePassword } from './registry.js';

export function runLoginStage(eventBus, root) {
  const users = getUsers();
  let selectedUserId = users[0]?.id || null;

  root.innerHTML = `
    <section class="lock-screen">
      <div class="clock" id="lock-clock"></div>
      <label for="user-list">User</label>
      <select id="user-list">
        ${users
          .map((user) => `<option value="${user.id}">${user.displayName}</option>`)
          .join('')}
      </select>
      <label for="password">Password</label>
      <input id="password" type="password" autocomplete="current-password" />
      <button id="login-button">Sign in</button>
      <p id="login-error" role="alert"></p>
    </section>
  `;

  const clock = root.querySelector('#lock-clock');
  const userList = root.querySelector('#user-list');
  const passwordInput = root.querySelector('#password');
  const loginButton = root.querySelector('#login-button');
  const errorBox = root.querySelector('#login-error');

  const renderClock = () => {
    const now = new Date();
    clock.textContent = now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  };

  renderClock();
  const clockTimer = setInterval(renderClock, 1000);

  userList?.addEventListener('change', (event) => {
    selectedUserId = event.target.value;
    errorBox.textContent = '';
  });

  const submit = () => {
    const password = passwordInput?.value || '';
    if (!selectedUserId || !validatePassword(selectedUserId, password)) {
      errorBox.textContent = 'Invalid credentials.';
      return;
    }

    clearInterval(clockTimer);
    eventBus.emit('auth:success', {
      userId: selectedUserId,
      authenticatedAt: new Date().toISOString(),
    });
  };

  loginButton?.addEventListener('click', submit);
  passwordInput?.addEventListener('keydown', (event) => {
    if (event.key === 'Enter') submit();
  });
}
