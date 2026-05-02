# User-mode Core (`user`)

## Ownership
* **Userland Core Team** owns session/process bootstrap and core runtime libraries.

## Build Targets
* `user_smss` (`STATIC`) in `user/smss`.
* `user_csrss` (`STATIC`) in `user/csrss`.
* `user_shell` (`STATIC`) in `user/shell`.
* `user_dll` (`STATIC`) in `user/dll`.

## Immediate Contributor Tasks
* Define subsystem startup sequence (`smss` -> `csrss` -> shell).
* Establish ABI and syscall stubs for core DLL projects.
* Add minimal host/unit tests for protocol contracts.
