export class Kernel {
  constructor() {
    this.subsystems = new Map();
    this.apps = new Map();
  }

  registerSubsystem(name, subsystem) {
    this.subsystems.set(name, subsystem);
  }

  registerApp(name, app) {
    this.apps.set(name, app);
  }

  async initialize() {
    for (const subsystem of this.subsystems.values()) {
      if (typeof subsystem.initialize === 'function') {
        await subsystem.initialize(this);
      }
    }

    for (const app of this.apps.values()) {
      if (typeof app.register === 'function') {
        await app.register(this);
      }
    }
  }
}
