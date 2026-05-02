# WFS On-Disk Format

## Overview
This document outlines the planned disk format for WFS, including metadata layout, record structure, journaling strategy, allocation bitmaps, and the attribute model.

## Superblock
The superblock defines filesystem identity and global geometry.

Planned fields:
- Magic + version
- Block size / cluster size
- Volume UUID
- Total blocks and free block counters
- MFT start location and record size
- Journal start, length, and sequence IDs
- Feature flags (compatible/incompatible/ro-compatible)
- Checksum

Redundancy:
- Primary superblock at fixed offset
- Secondary/backup copy in a reserved region

## MFT Records
WFS uses fixed-size Master File Table (MFT) records with attribute lists.

Record structure (planned):
- Record header (signature, record number, link count, flags)
- Base metadata (timestamps, ownership, mode/security bits)
- Attribute directory (typed attribute entries)
- Resident or non-resident data descriptors
- Optional extent/indirection metadata
- Record checksum/CRC

Attribute examples:
- Standard information
- Filename / directory index key
- Data stream(s)
- Security descriptor
- Reparse/object metadata (future)

## Journal
WFS journal is a write-ahead log for metadata consistency.

Planned behavior:
- Transaction begin/commit records
- Intent + redo records for metadata operations
- Sequence-numbered replay on mount
- Checkpointing to bound replay time
- Optional checksummed log records

Recovery model:
- Replay committed transactions not reflected in stable metadata.
- Discard incomplete/uncommitted transactions.

## Allocation Bitmap
Allocation state is tracked using block and possibly inode/record bitmaps.

Planned properties:
- Bitmap regions referenced from superblock.
- Fast scan hints for free-space discovery.
- Atomic/transactional updates via journal integration.
- Separate policies for metadata-preferred vs data allocation zones.

## Attribute Model
WFS stores object metadata as typed attributes attached to MFT records.

Design goals:
- Extensible typed attributes with versioning.
- Resident storage for small values; extent-backed for large values.
- Multiple named data streams per file (future-capable).
- Directory representation as indexed attributes.
- Explicit unknown-attribute handling rules for compatibility.

## Planned Source Files
- `fs/wfs/format.h` — Core on-disk structure definitions (superblock, record headers, attributes).
- `fs/wfs/superblock.c` (`wfs_sb_read`, `wfs_sb_validate`) — Superblock IO and validation logic.
- `fs/wfs/mft.c` (`wfs_mft_read_record`, `wfs_mft_write_record`) — MFT record parsing and persistence.
- `fs/wfs/attr.c` (`wfs_attr_find`, `wfs_attr_insert`) — Attribute enumeration and mutation.
- `fs/wfs/journal.c` (`wfs_journal_begin`, `wfs_journal_commit`, `wfs_journal_replay`) — WAL and recovery flow.
- `fs/wfs/bitmap.c` (`wfs_bitmap_alloc`, `wfs_bitmap_free`) — Allocation bitmap management.
- `tools/mkfs.wfs/main.c` (`mkfs_wfs`) — Filesystem image creation and initial metadata layout.
