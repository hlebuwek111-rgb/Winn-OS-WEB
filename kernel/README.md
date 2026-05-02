# Kernel Core (`kernel`)

## Ownership
* **Kernel Team** owns initialization, scheduler, memory manager, object manager, and process manager.

## Build Targets
* `kernel_core` (`STATIC` library): nucleus for executive subsystems linked into the kernel image.

## Planned Subsystems
* Early/late init sequence.
* Thread scheduler and dispatcher.
* Virtual/physical memory managers.
* Object namespace and handle table.
* Process/thread lifecycle and IPC foundations.

## Immediate Contributor Tasks
* Establish directory splits per subsystem (`init`, `sched`, `mm`, `ob`, `ps`).
* Define cross-subsystem contracts and lock hierarchy.
* Add bootstrap code paths consumed by `boot/uefi` handoff.
