# WFS On-Disk Format

This document captures the proposed on-disk layout and metadata model for WFS, focused on recoverability, typed attributes, and efficient namespace operations.

## Scope

* Superblock and global metadata
* MFT-like record structure
* Journal/WAL strategy
* Allocation bitmaps
* Attribute-based file model

## Disk Layout (Planned)

1. **Boot/Reserved Region**
   * Reserved sectors for future boot integration and format signatures.
2. **Primary Superblock**
   * Canonical filesystem metadata and feature flags.
3. **Superblock Mirror(s)**
   * Redundant copies for recovery.
4. **MFT Region**
   * Fixed-size records for files/directories/metadata objects.
5. **Allocation Metadata**
   * Block/cluster bitmap(s) and possibly inode/MFT bitmap.
6. **Journal Region**
   * Write-ahead log for metadata transactions.
7. **Data Region**
   * File content extents and overflow attribute storage.

## Superblock

Proposed fields:

* Magic + version
* Block size / cluster size
* Total blocks, free blocks
* MFT start + length
* Journal start + length
* Bitmap start + length
* Root directory record ID
* Volume UUID and label
* Compatible/incompatible feature flags
* Checksum

Design goals:

* Single-structure parse for mount fast-path.
* Explicit feature gating for forward compatibility.
* Checksum validation before trust.

## MFT Records

Each object is represented by one base record with optional extension records.

Proposed record header:

* Record magic + sequence
* Record ID
* Flags (in-use, directory, system, extent, deleted)
* Link count
* Attribute table offset/length
* Base/extension linkage
* Record checksum

Directory objects use index attributes mapping names to child record IDs.

## Attribute Model

WFS uses typed attributes per record (inspired by NT-style metadata streams):

* `STANDARD_INFORMATION` (timestamps, flags, ownership)
* `FILE_NAME` (parent ID, Unicode name, namespace flags)
* `DATA` (resident or non-resident extents)
* `INDEX_ROOT` / `INDEX_ALLOCATION` (directory indexing)
* `SECURITY_DESCRIPTOR` (optional phase)
* `REPARSE_POINT` / `EA` (future optional features)

Resident attributes fit inside record body; non-resident attributes store extent runs.

## Journal Model

Journal is metadata-first WAL with transaction framing:

* Begin transaction record
* One or more redo/undo metadata delta records
* Commit record

Recovery on mount:

* Scan from last checkpoint.
* Reapply committed transactions not yet checkpointed.
* Roll back incomplete transactions using undo entries.

## Bitmap Allocation

Block allocation state is tracked by bitmap pages:

* Bit = allocated/free cluster status.
* Optional segregated bitmaps for metadata vs user data.
* Atomic updates logged through journal before checkpoint.

## Planned Source Files

* `fs/wfs/format.h` — on-disk structs, constants, feature flags.
* `fs/wfs/superblock.c` — `wfs_read_superblock`, `wfs_validate_superblock`.
* `fs/wfs/mft.c` — `wfs_mft_read_record`, attribute enumeration/update.
* `fs/wfs/attr.c` — attribute parsing, resident/non-resident handling.
* `fs/wfs/bitmap.c` — allocation bitmap load, allocate/free operations.
* `fs/wfs/journal.c` — WAL append, replay, checkpoint.
* `fs/wfs/recovery.c` — mount-time recovery orchestration.
