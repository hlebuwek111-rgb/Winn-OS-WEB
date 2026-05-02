# UEFI Bootloader Module (`boot/uefi`)

## Ownership
* **Boot Team** owns EDK II integration, firmware handoff, and boot-time memory map policy.

## Build Targets
* `boot_uefi` (custom target): root integration point for EDK II build orchestration.

## Planned Contents
* EDK II DSC/FDF files.
* PEI/DXE drivers needed by Winn OS.
* Boot manager and kernel loader handoff stubs.

## Immediate Contributor Tasks
* Introduce EDK II workspace layout under this directory.
* Add CMake glue to call EDK II builds reproducibly.
* Define artifacts consumed by image assembly (`BOOTX64.EFI`, config blobs, symbols).
