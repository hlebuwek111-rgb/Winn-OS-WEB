# Winn-OS-WEB

Winn-OS-WEB is a static web project prepared for continuous integration and GitHub Pages deployment.

## Required Setup

### 1) Repository settings (GitHub)

1. Go to **Settings → Pages**.
2. Under **Build and deployment**, set source to **GitHub Actions**.
3. Ensure your default deployment branch is `main`.

### 2) Local prerequisites

Install the following tools:

- **Git** (latest stable)
- **Node.js** 20+ and **npm** (only required if/when `package.json` lint scripts are added)
- Optional: **Python 3** (handy for local static file serving)

### 3) Branching and CI behavior

- On every push and pull request, `.github/workflows/deploy.yml` runs lint checks.
- On push to `main`, the same workflow deploys the static site to GitHub Pages.

## Local Run Steps

### Quick local preview (no build step)

From the repository root:

```bash
python3 -m http.server 8080
```

Then open `http://localhost:8080`.

### Run local lint checks

If a `package.json` exists with lint scripts:

```bash
npm ci
npm run lint
```

If `package.json` does not exist yet, lint is intentionally skipped by CI.

## CI/CD Workflow

Workflow file: `.github/workflows/deploy.yml`

- **lint job**
  - Checks out code.
  - Runs YAML linting.
  - Runs `npm run lint` when `package.json` is present.
- **deploy job**
  - Runs only on pushes to `main`.
  - Packages the repository (excluding CI/system folders) into a Pages artifact.
  - Deploys via official GitHub Pages Actions.

## Project Structure

Current scaffold:

```text
.
├── .github/
│   ├── .yamllint.yml
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md
│   │   └── feature_request.md
│   └── workflows/
│       └── deploy.yml
├── LICENSE
├── README.md
└── .gitignore
```

Planned application structure:

```text
.
├── public/
│   ├── index.html
│   └── assets/
├── src/
│   ├── modules/
│   │   ├── core/
│   │   │   ├── app-shell.js
│   │   │   └── router.js
│   │   ├── auth/
│   │   │   ├── auth-service.js
│   │   │   └── session-store.js
│   │   ├── dashboard/
│   │   │   ├── dashboard-view.js
│   │   │   └── widgets/
│   │   └── settings/
│   │       ├── settings-view.js
│   │       └── preferences-store.js
│   └── styles/
│       └── main.css
└── tests/
    ├── unit/
    └── e2e/
```

## Phased Roadmap (Mapped to Module Files)

### Phase 1 — Foundation

- Add static shell and routing bootstrap:
  - `src/modules/core/app-shell.js`
  - `src/modules/core/router.js`
- Establish base styling:
  - `src/styles/main.css`

### Phase 2 — Authentication

- Implement login/session management:
  - `src/modules/auth/auth-service.js`
  - `src/modules/auth/session-store.js`
- Connect auth state to shell/router:
  - `src/modules/core/app-shell.js`
  - `src/modules/core/router.js`

### Phase 3 — Dashboard Experience

- Build dashboard UI and data widgets:
  - `src/modules/dashboard/dashboard-view.js`
  - `src/modules/dashboard/widgets/*`
- Add unit test coverage:
  - `tests/unit/dashboard/*.test.js`

### Phase 4 — Settings and Preferences

- Add settings screens and persistence:
  - `src/modules/settings/settings-view.js`
  - `src/modules/settings/preferences-store.js`
- Expand route coverage and integration tests:
  - `src/modules/core/router.js`
  - `tests/e2e/settings.spec.js`

### Phase 5 — Hardening and Release

- Improve accessibility and performance across modules.
- Add lint/test quality gates as required scripts in `package.json`.
- Introduce release notes and deployment validation checks.

## Issue Templates

Use the included templates for consistent issue intake:

- Bug report: `.github/ISSUE_TEMPLATE/bug_report.md`
- Feature request: `.github/ISSUE_TEMPLATE/feature_request.md`

## Contributing

1. Create a feature branch.
2. Make changes and run local checks.
3. Open a pull request using the repository templates/workflows.
