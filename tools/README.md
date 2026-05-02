# Build and Image Tools (`tools`)

## Ownership
* **Build Tooling Team** owns host-side utilities and image assembly scripts.

## Build Targets
* `mkbootfs` (custom target): boot filesystem packer utility.
* `mkfs_wfs` (custom target): WFS image formatter (`mkfs.wfs`).
* `build_iso` (custom target): ISO creation pipeline (`build_iso.py`).

## Planned Contents
* `mkbootfs/` project source.
* `mkfs.wfs/` project source.
* `build_iso.py` and helper modules.

## Immediate Contributor Tasks
* Decide implementation languages and runtime requirements.
* Add CLI contracts shared with CI image-generation pipeline.
* Provide reproducible integration tests using sample manifests.
