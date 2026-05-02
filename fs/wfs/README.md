# Winn Filesystem (`fs/wfs`)

## Ownership
* **WFS Maintainers** own on-disk format specs and driver implementation.

## Build Targets
* `wfs_driver` (`STATIC` library): WFS parser, allocator, and runtime operations.

## Planned Contents
* On-disk superblock/inode/extent structures.
* Format checker and recovery policies.
* Kernel driver integration for mount/read/write.

## Immediate Contributor Tasks
* Write canonical on-disk structure definitions with endianness rules.
* Add serializer/deserializer tests shared with `tools/mkfs.wfs`.
* Define compatibility/versioning strategy for early images.
