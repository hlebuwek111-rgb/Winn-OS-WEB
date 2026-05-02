# Boot Sequence

## Overview
This document defines the intended boot flow from UEFI firmware into the Winn kernel, including the handoff contract and memory map ownership transfer.

## UEFI Handoff
1. **Firmware entry**: UEFI loads the bootloader image and calls its entry point.
2. **Boot services phase**: The bootloader discovers platform services, obtains the system memory map, and loads the kernel image plus required modules.
3. **Graphics/console setup**: The bootloader establishes a basic framebuffer or text console contract for early diagnostics.
4. **ExitBootServices transition**: The bootloader calls `ExitBootServices` with a valid map key and transitions ownership of physical memory/resources to the OS.
5. **Kernel transfer**: Control is transferred to the kernel entry point with a stable boot info structure.

## Memory Map Contract
The bootloader must provide a normalized memory map to the kernel, preserving original UEFI types and an OS-specific classification.

Expected map categories:
- Usable RAM
- Reserved / firmware-owned
- ACPI reclaimable and ACPI NVS
- MMIO / device regions
- Bootloader/runtime allocations
- Kernel image + bootstrap structures

The kernel must treat all memory as unavailable until consumed by the PMM during initialization.

## Kernel Entry Contract
The kernel entry ABI should include:
- Pointer to boot info header (versioned)
- Physical/virtual base addresses for kernel image
- Memory map pointer + entry size/count
- Framebuffer descriptor (optional but preferred)
- RSDP pointer for ACPI discovery
- Initrd/module list pointer (optional)
- Boot CPU information (xAPIC ID, feature bits)

Contract requirements:
- All pointers are valid at entry and mapped as documented.
- Structure version is checked before use.
- Bootloader and kernel must agree on page size, alignment, and calling convention.

## Failure Modes
- `ExitBootServices` retry required due to stale map key.
- Missing required descriptors (memory map/RSDP) should hard-fail with diagnostic output.
- ABI version mismatch should stop boot before touching global state.

## Planned Source Files
- `boot/uefi/main.c` — UEFI image entry, service discovery, and staged boot control.
- `boot/uefi/memory_map.c` — Capture, normalize, and package firmware memory descriptors.
- `boot/uefi/handoff.c` — Build boot info structures and execute `ExitBootServices`.
- `boot/common/boot_info.h` — Versioned boot info schema shared with kernel.
- `kernel/arch/x86_64/entry.S` — Early assembly entry and register contract bridge.
- `kernel/arch/x86_64/kernel_entry.c` — C-level kernel entry and validation of boot handoff data.
