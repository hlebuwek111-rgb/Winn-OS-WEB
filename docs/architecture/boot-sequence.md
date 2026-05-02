# Boot Sequence

This document defines the intended boot flow from UEFI firmware into the Winn kernel and the minimum handoff contract required for deterministic startup.

## Scope

* UEFI application startup and services usage
* Physical memory map acquisition and normalization
* ExitBootServices transition
* Kernel image placement and control transfer
* Kernel entry contract (registers + boot info)

## Boot Flow (Planned)

1. **UEFI Entry (`efi_main`)**
   * Validate System Table pointers and console output.
   * Locate required protocols (Loaded Image, Simple File System, Graphics Output where applicable).
2. **Load Kernel Image**
   * Open kernel file from EFI System Partition.
   * Parse executable format (planned: PE/COFF or ELF; final decision pending).
   * Allocate pages for segments and perform relocation/fixups.
3. **Acquire Platform Information**
   * Read UEFI memory map.
   * Capture framebuffer/console info.
   * Gather ACPI RSDP pointer and other firmware tables needed by early kernel init.
4. **Finalize Boot Metadata**
   * Build a compact `boot_info` structure in reserved memory.
   * Convert UEFI descriptors into internal memory-region types.
5. **Exit Boot Services**
   * Refresh memory map and obtain current map key.
   * Call `ExitBootServices` with the latest key.
   * Ensure no further UEFI boot services calls occur after success.
6. **Transfer Control to Kernel**
   * Set up architecture-defined entry state (stack, paging mode assumptions, interrupts disabled).
   * Jump to kernel entry point with pointer to `boot_info`.

## UEFI Handoff Requirements

* Firmware-provided memory map must be copied to kernel-owned memory before exiting boot services.
* Kernel image and `boot_info` pages must be marked reserved in the memory map passed forward.
* Bootloader must preserve ACPI table pointers and graphics mode data if present.
* Bootloader must guarantee 64-bit long mode entry contract for x86_64 kernel targets.

## Memory Map Contract

The bootloader must pass a normalized memory map consisting of:

* Base physical address
* Region length
* Region type (`usable`, `reserved`, `acpi_reclaim`, `acpi_nvs`, `mmio`, `runtime`, `bad`)
* Optional attributes bitfield (cacheability/runtime flags)

Kernel expectations:

* Entries are page-aligned (4 KiB minimum granularity).
* Overlapping entries are resolved before handoff.
* Usable regions exclude bootloader image, kernel image, page tables, and handoff structures.

## Kernel Entry Contract

The kernel entry ABI should be stable and versioned.

**Proposed x86_64 contract:**

* `RDI` = physical (or identity-mapped virtual) pointer to `boot_info`
* `RSI` = `boot_info` size in bytes
* `RSP` = valid aligned stack provided by bootloader/early trampoline
* Interrupts disabled (`IF=0`)
* CPU in long mode with paging enabled

`boot_info` should include at least:

* Memory map descriptor array
* Kernel physical/virtual load bounds
* ACPI RSDP pointer
* Framebuffer descriptor (if graphics mode active)
* Bootloader version + contract version fields

## Failure Behavior

If required preconditions fail (kernel image invalid, memory map unavailable, ExitBootServices failure loop), bootloader should:

* emit diagnostics to console/serial,
* halt cleanly,
* avoid partial handoff to kernel.

## Planned Source Files

* `boot/uefi/main.c` — `efi_main`, protocol discovery, boot flow coordinator.
* `boot/uefi/loader.c` — `load_kernel_image`, segment mapping, relocation.
* `boot/uefi/memory_map.c` — `capture_memory_map`, descriptor normalization.
* `boot/uefi/handoff.c` — `build_boot_info`, contract version stamping.
* `boot/uefi/exit_boot.c` — `exit_boot_services_with_retry`.
* `kernel/arch/x86_64/entry.S` — low-level entry stub and register contract bridge.
* `kernel/arch/x86_64/boot/boot_info.h` — shared handoff structure definitions.
