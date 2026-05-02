# Building WinnOS

This document describes a reproducible host-side build for WinnOS.

## Host prerequisites

Tested on Ubuntu 24.04 and Debian 12.

Install required packages:

```bash
sudo apt-get update
sudo apt-get install -y \
  clang lld llvm \
  cmake ninja-build python3 \
  dosfstools mtools gdisk xorriso
```

Tooling used by this repository:

- `clang`, `clang++`, `lld-link`, `llvm-lib`
- `cmake` + `ninja`
- `sgdisk`, `mkfs.vfat`, `mcopy`, `truncate`
- `python3`

## Configure and build

From repository root:

```bash
cmake -S . -B build \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/x86_64-pc-winnos.cmake \
  -DWINNOS_BUILD_BOOTLOADER=ON \
  -DWINNOS_BUILD_KERNEL=ON \
  -DWINNOS_BUILD_DRIVERS=ON \
  -DWINNOS_BUILD_USER=ON

cmake --build build -- -v
```

## Build image artifacts

Assuming outputs are produced at:

- `build/bootloader/BOOTX64.EFI`
- `build/kernel/kernel.bin`
- `sysroot/user/`

Run:

```bash
python3 tools/build_iso.py \
  --bootloader build/bootloader/BOOTX64.EFI \
  --kernel build/kernel/kernel.bin \
  --user-root sysroot/user \
  --out-dir out/image
```

Expected outputs:

- `out/image/winnos.raw` (GPT disk image placeholder)
- `out/image/esp.img` (formatted FAT32 ESP)
- `out/image/boot.wfs` (formatted WFS placeholder)
- `out/image/recovery.wfs` (formatted WFS placeholder)

## Reproducibility notes

- Pin compiler and host packages (e.g. using a container image).
- Keep `CMAKE_TOOLCHAIN_FILE` fixed to avoid host ABI drift.
- Use deterministic input trees for `--user-root`.
- Future work: deterministic timestamps in filesystem builders and final ISO packager.
