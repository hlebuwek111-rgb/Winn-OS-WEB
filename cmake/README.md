# CMake Infrastructure

## Ownership
* **Build/Release Engineering** owns this directory.

## Targets and Responsibilities
* Stores reusable CMake modules and toolchain helpers shared by all modules.
* Keeps host/target detection logic, compile options, and packaging glue.

## Immediate Contributor Tasks
* Add `Toolchain-*.cmake` files for supported cross-compilers.
* Add reusable helper modules (for example `WinnCommonFlags.cmake`).
* Wire image packaging helpers as the boot and filesystem tools mature.
