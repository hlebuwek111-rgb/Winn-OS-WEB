export function mountDesktop(root, currentUser) {
  root.innerHTML = `
    <section class="desktop">
      <h1>Welcome, ${currentUser.displayName}</h1>
      <div class="wallpaper"></div>
    </section>
  `;
}
