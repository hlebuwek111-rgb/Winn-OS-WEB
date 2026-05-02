# Software Development Kit (`sdk`)

## Ownership
* **SDK/API Team** owns public headers, import libraries, and developer-facing ABI policy.

## Build Targets
* `winn_sdk` (`INTERFACE`): umbrella target for exported include paths and import-lib metadata.

## Immediate Contributor Tasks
* Add `include/` tree for stable public headers.
* Define import library generation flow for user-mode binaries.
* Document API versioning and deprecation policy.
