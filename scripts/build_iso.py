#!/usr/bin/env python3
"""Build a placeholder GPT image for Winn OS Native."""

from pathlib import Path


def main() -> None:
    out = Path("build/winnos.img")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(b"WINN-OS-IMAGE-PLACEHOLDER")
    print(f"Created {out}")


if __name__ == "__main__":
    main()
