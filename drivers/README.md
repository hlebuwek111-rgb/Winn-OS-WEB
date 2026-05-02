# Drivers (`drivers`)

## Ownership
* **Device Enablement Team** owns in-tree driver frameworks and class drivers.

## Build Targets
* `driver_storage` (`STATIC`) in `drivers/storage`.
* `driver_video` (`STATIC`) in `drivers/video`.
* `driver_input` (`STATIC`) in `drivers/input`.
* `driver_bus` (`STATIC`) in `drivers/bus`.

## Immediate Contributor Tasks
* Define shared driver model contracts (PnP, power, DMA mapping, IRP equivalents).
* Add bus enumeration plumbing first (PCI/ACPI roots).
* Land class-driver skeletons and test doubles for early bring-up.
