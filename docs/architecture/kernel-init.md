# Kernel Initialization Order

This document outlines the planned deterministic initialization order for core kernel subsystems, with emphasis on interrupt safety and dependency sequencing.

## Scope

* CPU/descriptor-table bring-up
* Interrupt controller initialization
* Physical and virtual memory initialization
* Scheduler and first task activation

## Initialization Phases (Planned)

1. **Early CPU Context**
   * Enter C runtime from assembly trampoline.
   * Validate and pin `boot_info`.
   * Initialize early serial/log output.
2. **GDT/TSS Setup**
   * Build and load kernel GDT.
   * Initialize TSS and IST stacks for fault handlers.
   * Reload segment selectors.
3. **IDT Setup**
   * Populate exception vectors.
   * Register early IRQ stubs (masked/disabled initially).
   * Load IDT (`lidt`).
4. **APIC/Interrupt Controller Bring-up**
   * Disable legacy PIC (if x86 legacy path enabled).
   * Initialize Local APIC.
   * Configure IOAPIC redirection entries for timer/keyboard/serial baselines.
   * Keep interrupts disabled until handlers and scheduler clock are ready.
5. **PMM (Physical Memory Manager)**
   * Consume normalized memory map from bootloader.
   * Build frame allocator (bitmap or buddy backend TBD).
   * Reserve kernel/boot structures and device regions.
6. **VMM (Virtual Memory Manager)**
   * Establish kernel address-space layout.
   * Build page-table manager and mapping APIs.
   * Map heap, direct-map (if used), MMIO windows, and guard pages.
7. **Kernel Heap / Object Allocators**
   * Bring up general-purpose allocator(s) on top of VMM+PMM.
8. **Timer + Scheduler**
   * Initialize monotonic clock source (LAPIC timer/HPET/TSC deadline path TBD).
   * Initialize run queues and scheduler policy state.
   * Create idle thread + first kernel worker/init thread.
9. **Enable Interrupts + Dispatch**
   * Unmask required IRQ lines.
   * Execute `sti` after scheduler and timer handlers are fully installed.
10. **Subsystem Expansion**
    * Initialize VFS, WFS, syscall layer, process manager, driver manager.
    * Transition to user-mode bootstrap when available.

## Dependency Notes

* IDT must be valid before any interrupt source is enabled.
* PMM must be available before dynamic VMM mappings beyond bootstrap tables.
* Scheduler depends on timer interrupts and heap allocation.
* User-mode entry depends on syscall/exception gates and per-task address spaces.

## Planned Source Files

* `kernel/arch/x86_64/entry.S` — assembly trampoline into `kmain`.
* `kernel/main/kmain.c` — top-level init sequencing and phase transitions.
* `kernel/arch/x86_64/gdt.c` — `gdt_init`, TSS setup/load.
* `kernel/arch/x86_64/idt.c` — `idt_init`, ISR/IRQ gate registration.
* `kernel/arch/x86_64/apic.c` — `lapic_init`, `ioapic_init`, PIC masking.
* `kernel/mm/pmm.c` — `pmm_init`, frame reserve/allocate/free.
* `kernel/mm/vmm.c` — `vmm_init`, map/unmap/protection APIs.
* `kernel/mm/heap.c` — kernel heap bootstrap.
* `kernel/sched/scheduler.c` — `sched_init`, first runnable thread setup.
* `kernel/time/timer.c` — timer source init and scheduler tick hookup.
