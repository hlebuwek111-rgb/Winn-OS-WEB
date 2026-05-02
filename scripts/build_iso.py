#!/usr/bin/env python3
"""Build WinnOS installation/boot media image.

Placeholder script that documents expected input artifacts and partition layout.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class ArtifactPaths:
    bootloader: Path
    kernel: Path
    drivers_dir: Path


def expected_artifacts(build_dir: Path) -> ArtifactPaths:
    artifacts = build_dir / "artifacts"
    return ArtifactPaths(
        bootloader=artifacts / "boot" / "EFI" / "BOOT" / "BOOTX64.EFI",
        kernel=artifacts / "kernel" / "winnoskrnl.exe",
        drivers_dir=artifacts / "drivers",
    )


def describe_partition_layout() -> str:
    return (
        "GPT layout (first pass):\n"
        "  1) EFI System Partition (FAT32, ~128 MiB)\n"
        "     - /EFI/BOOT/BOOTX64.EFI\n"
        "     - /EFI/WINNOS/winnoskrnl.exe\n"
        "     - /EFI/WINNOS/drivers/*.lib (temporary placeholder)\n"
        "  2) WinnOS System Partition (WFS, remaining space)\n"
        "     - future root filesystem payload\n"
    )


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    build_dir = root / "build"

    artifacts = expected_artifacts(build_dir)
    print("WinnOS ISO build placeholder")
    print(f"Repository root: {root}")
    print(f"Expected bootloader: {artifacts.bootloader}")
    print(f"Expected kernel: {artifacts.kernel}")
    print(f"Expected drivers dir: {artifacts.drivers_dir}")
    print(describe_partition_layout())
    print("TODO: generate FAT image, package GPT disk image, and emit ISO/IMG artifacts.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
