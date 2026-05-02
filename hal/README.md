# Hardware Abstraction Layer (`hal`)

## Ownership
* **Platform Team** owns architecture- and board-specific abstractions.

## Build Targets
* `hal` (`INTERFACE` library): exported contract for timer, interrupt, MMU, SMP, and low-level IO APIs.

## Planned Contents
* `include/hal/*` public kernel-facing interfaces.
* `arch/<arch>` and `platform/<platform>` implementations.

## Immediate Contributor Tasks
* Define interrupt controller and clock source interfaces.
* Add paging/MMU abstraction contracts for x86_64 first.
* Establish initialization sequencing from early boot to scheduler-ready state.
