# WFS on-disk format notes

## Binary layout baseline

WFS currently expects the first bytes of a volume image to contain a fixed-size
`struct wfs_superblock` (512 bytes). All fields are little-endian and all
structures in `include/wfs_ondisk.h` are marked `__attribute__((packed))` to
remove compiler-introduced padding.

### Superblock fields

- `magic` must be `WFS_SUPERBLOCK_MAGIC` (`0x53465757`).
- `version_major` must match `WFS_SUPERBLOCK_VERSION`.
- `bytes_per_sector` and `sectors_per_cluster` must be powers of two.
- Cluster size (`bytes_per_sector * sectors_per_cluster`) must fall in
  `[512, 2 MiB]`.
- `mft_lcn`, `journal_lcn`, and `bitmap_lcn` are logical cluster numbers and
  must resolve inside total volume size.

## MFT and attributes

MFT records begin with `struct wfs_mft_record_header` followed by a sequence of
attributes. Each attribute begins with `struct wfs_attr_header` and then uses
resident/non-resident tails (`struct wfs_attr_resident` or
`struct wfs_attr_nonresident`). Extent mappings use `struct wfs_extent_desc`.

## Alignment requirements

Because these definitions are consumed by raw-image parsers and low-level mount
code, code must not assume natural alignment of fields in memory. Use explicit
byte-wise I/O into the packed structs and avoid direct pointer casting from
unaligned buffers unless the target architecture allows it safely.

## Host inspection utility

`tools/wfs_inspect/main.c` implements a static parser that reads the superblock
from a raw image and prints structural fields for quick diagnostics.
