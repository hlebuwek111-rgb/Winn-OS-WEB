# Filesystems (`fs`)

## Ownership
* **Storage & FS Team** owns VFS contracts and filesystem implementations.

## Build Targets
* Delegates to per-filesystem targets, currently `wfs_driver` under `fs/wfs`.

## Immediate Contributor Tasks
* Define VFS-facing interface boundaries.
* Standardize on-disk metadata validation utilities.
* Add mount/format integration hooks for system initialization.
