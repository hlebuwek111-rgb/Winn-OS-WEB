#!/usr/bin/env python3
"""Build a WinnOS disk/ISO staging artifact.

This script currently creates a GPT-partitioned raw disk image with:
- ESP      (FAT32)
- Boot     (WFS placeholder)
- Recovery (WFS placeholder)

It then mounts/stages files and prepares artifacts for ISO generation.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def run(cmd: list[str]) -> None:
    print("+", " ".join(cmd))
    subprocess.run(cmd, check=True)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Build WinnOS GPT image and stage system files")
    parser.add_argument("--bootloader", required=True, type=Path, help="Path to bootloader .efi")
    parser.add_argument("--kernel", required=True, type=Path, help="Path to kernel binary")
    parser.add_argument("--user-root", required=True, type=Path, help="Directory with user-space files")
    parser.add_argument("--out-dir", default=Path("out/image"), type=Path, help="Output directory")
    parser.add_argument("--disk-size-mib", default=1024, type=int, help="Disk image size in MiB")
    parser.add_argument("--esp-size-mib", default=128, type=int, help="ESP size in MiB")
    parser.add_argument("--boot-size-mib", default=512, type=int, help="Boot partition size in MiB")
    parser.add_argument("--recovery-size-mib", default=256, type=int, help="Recovery partition size in MiB")
    return parser.parse_args()


def ensure_tools() -> None:
    required = ["sgdisk", "mkfs.vfat", "mcopy", "truncate"]
    missing = [tool for tool in required if shutil.which(tool) is None]
    if missing:
        raise RuntimeError(f"Missing required host tools: {', '.join(missing)}")


def main() -> int:
    args = parse_args()
    ensure_tools()

    out_dir = args.out_dir.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    disk = out_dir / "winnos.raw"
    esp_img = out_dir / "esp.img"
    boot_img = out_dir / "boot.wfs"
    recovery_img = out_dir / "recovery.wfs"
    staging = out_dir / "staging"

    for path in [disk, esp_img, boot_img, recovery_img]:
        if path.exists():
            path.unlink()

    total = args.disk_size_mib
    run(["truncate", "-s", f"{total}M", str(disk)])

    run(["sgdisk", "-o", str(disk)])
    run(["sgdisk", "-n", f"1:1MiB:+{args.esp_size_mib}MiB", "-t", "1:EF00", "-c", "1:ESP", str(disk)])
    run(["sgdisk", "-n", f"2:0:+{args.boot_size_mib}MiB", "-t", "2:8300", "-c", "2:BOOT", str(disk)])
    run(["sgdisk", "-n", f"3:0:+{args.recovery_size_mib}MiB", "-t", "3:8300", "-c", "3:RECOVERY", str(disk)])

    run(["truncate", "-s", f"{args.esp_size_mib}M", str(esp_img)])
    run(["mkfs.vfat", "-F", "32", "-n", "WINNOS_ESP", str(esp_img)])

    run([
        "python3",
        str(Path(__file__).with_name("mkfs.wfs") / "mkfs.wfs"),
        str(boot_img),
        "--label",
        "WINNOS_BOOT",
        "--force",
    ])
    run([
        "python3",
        str(Path(__file__).with_name("mkfs.wfs") / "mkfs.wfs"),
        str(recovery_img),
        "--label",
        "WINNOS_RECOVERY",
        "--force",
    ])

    if staging.exists():
        shutil.rmtree(staging)
    (staging / "EFI/BOOT").mkdir(parents=True)
    (staging / "system").mkdir(parents=True)

    shutil.copy2(args.bootloader, staging / "EFI/BOOT/BOOTX64.EFI")
    shutil.copy2(args.kernel, staging / "system/kernel.bin")
    if args.user_root.exists():
        shutil.copytree(args.user_root, staging / "system/user", dirs_exist_ok=True)

    run(["mcopy", "-i", str(esp_img), "-s", str(staging / "EFI"), "::"]) 

    print("Created:")
    print(f"  GPT disk image: {disk}")
    print(f"  ESP image:      {esp_img}")
    print(f"  Boot WFS:       {boot_img}")
    print(f"  Recovery WFS:   {recovery_img}")
    print("TODO: copy partition images into GPT disk and emit final ISO9660 artifact.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
