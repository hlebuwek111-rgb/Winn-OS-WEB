# Winn-OS-WEB

Baseline repository layout and CMake superbuild for WinnOS.

## Prerequisites

Install the following tools on your host:

- CMake 3.24+
- Ninja (recommended generator)
- Clang/Clang++
- LLD (`lld-link`)
- NASM
- Python 3.10+

> Note: this tree is configured for a custom target triple: `x86_64-pc-winnos`.

## First-pass build flow

```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-winnos.cmake
cmake --build build
python3 scripts/build_iso.py
```

## Repository structure

- `boot/` - UEFI bootloader target (`BOOTX64.EFI`)
- `kernel/` - kernel image target (`winnoskrnl.exe`)
- `hal/` - hardware abstraction layer scaffolding
- `drivers/` - initial driver stub libraries
- `fs/wfs/` - WFS filesystem module scaffolding
- `user/` - userland program scaffolding
- `tools/` - host tooling scaffolding
- `include/` - shared headers
- `cmake/` - toolchain and CMake support files
- `scripts/` - build/packaging scripts (ISO script placeholder)
