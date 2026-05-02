# Winn OS Native (Bootstrap Tree)

This repository now contains the initial bootstrap source tree for **Winn OS Native**, including:

- UEFI bootloader assembly entry stub
- Kernel entry path and architecture initialization stubs
- Early serial driver
- WFS on-disk structure definitions + skeleton driver
- Minimal user-mode shell prototype
- Initial CMake project structure and ISO build placeholder script

## Quick start

```bash
cmake -S . -B build
cmake --build build
python3 scripts/build_iso.py
```

> Note: this is an initial scaffolding milestone and not yet a fully bootable production OS.
