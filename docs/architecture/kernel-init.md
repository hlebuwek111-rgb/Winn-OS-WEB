# Kernel Initialization Order

## Overview
This document defines the expected initialization sequence after kernel entry on x86_64, with emphasis on ordering constraints across descriptor tables, interrupt controllers, and memory/scheduling subsystems.

## Initialization Sequence
1. **Early CPU bring-up**
   - Establish temporary stack and per-CPU bootstrap context.
   - Validate CPU feature baseline (long mode assumptions, APIC availability, etc.).

2. **GDT initialization**
   - Install kernel code/data segments and task state segment descriptors.
   - Load GDTR and perform required segment register reloads.

3. **IDT initialization**
   - Populate exception vectors first (fault-safe handlers).
   - Populate interrupt vectors for timers/IPIs/devices as stubs.
   - Load IDTR.

4. **APIC/interrupt controller setup**
   - Discover APIC topology (via ACPI/MADT).
   - Enable local APIC on BSP.
   - Configure timer interrupt source and spurious vector.
   - Mask/disable legacy PIC if present.

5. **PMM (Physical Memory Manager) init**
   - Consume boot memory map.
   - Build frame allocator state (bitmap/stack/tree model TBD).
   - Reserve kernel, boot structures, and MMIO ranges.

6. **VMM (Virtual Memory Manager) init**
   - Build/lock kernel page tables.
   - Map higher-half kernel regions and direct-map window (if adopted).
   - Enable heap/bootstrap virtual allocators.

7. **Scheduler init**
   - Initialize run queues and core task structures.
   - Create idle task and initial kernel thread(s).
   - Start timer-driven preemption after interrupt path is verified.

8. **Subsystem handoff**
   - Transition from bootstrap allocators to steady-state allocators.
   - Bring up drivers, VFS, and userspace launch path.

## Ordering Constraints
- IDT must be valid before enabling interrupt delivery.
- APIC timer usage depends on stable vector table and handler stubs.
- PMM must precede dynamic VMM expansion.
- Scheduler preemption must remain disabled until timer and context-switch paths are proven safe.

## Planned Source Files
- `kernel/arch/x86_64/entry.S` — Earliest entry, stack setup, and jump into C init path.
- `kernel/arch/x86_64/gdt.c` (`gdt_init`) — GDT/TSS creation and load.
- `kernel/arch/x86_64/idt.c` (`idt_init`) — Exception/IRQ vector table setup.
- `kernel/arch/x86_64/apic.c` (`apic_init`, `apic_timer_init`) — Local APIC enablement and timer configuration.
- `kernel/mm/pmm.c` (`pmm_init`, `pmm_alloc_frame`) — Physical frame allocator initialization and API.
- `kernel/mm/vmm.c` (`vmm_init`, `vmm_map`, `vmm_unmap`) — Kernel virtual memory subsystem.
- `kernel/sched/scheduler.c` (`sched_init`, `sched_tick`, `sched_switch`) — Core scheduling lifecycle.
- `kernel/init/main.c` (`kernel_init`) — Top-level orchestration of the initialization sequence.
