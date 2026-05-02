# Syscall Surface (Initial Nt* API)

This document defines the planned first-pass syscall boundary and kernel/user contract for Winn OS.

## Scope

* Syscall ABI entry mechanism
* Initial `Nt*` syscall families
* Handle/object model assumptions
* Validation, security, and error contract

## User/Kernel Boundary

### Entry Mechanism (Planned)

* x86_64 fast syscall path via `SYSCALL/SYSRET`.
* User stubs in `ntdll`-like runtime marshal arguments and syscall numbers.
* Kernel dispatcher validates caller mode, copies user buffers safely, then routes to service handlers.

### ABI Principles

* Stable syscall numbers gated by interface versioning.
* Explicit pointer provenance checks for user-space addresses.
* Structured status returns (`NTSTATUS`-style codes).
* No direct kernel pointer exposure to user mode.

## Initial Nt* Surface (Planned)

### Process/Thread

* `NtCreateProcess`
* `NtTerminateProcess`
* `NtCreateThread`
* `NtTerminateThread`
* `NtYieldExecution`

### Virtual Memory

* `NtAllocateVirtualMemory`
* `NtFreeVirtualMemory`
* `NtProtectVirtualMemory`
* `NtMapViewOfSection`
* `NtUnmapViewOfSection`

### Object/Handle Core

* `NtClose`
* `NtDuplicateObject`
* `NtQueryObject`

### File I/O (WFS-backed)

* `NtCreateFile`
* `NtReadFile`
* `NtWriteFile`
* `NtQueryInformationFile`
* `NtSetInformationFile`

### Synchronization / IPC (Minimal)

* `NtCreateEvent`
* `NtSetEvent`
* `NtWaitForSingleObject`

### System Information

* `NtQuerySystemInformation`
* `NtQueryPerformanceCounter`

## Validation Rules

Every syscall handler should:

1. Validate access mode and handle rights.
2. Probe/copy user buffers with fault-safe helpers.
3. Fail closed on malformed structures or size mismatches.
4. Return deterministic status codes for auditability.

## Planned Source Files

* `kernel/arch/x86_64/syscall_entry.S` — low-level syscall entry/exit stubs.
* `kernel/sys/syscall_dispatch.c` — syscall table, number validation, dispatch.
* `kernel/sys/ntstatus.h` — canonical status code definitions.
* `kernel/ob/object.c` — handle table/object reference management.
* `kernel/mm/sys_vm.c` — virtual memory syscall backends.
* `kernel/ps/sys_process.c` — process/thread syscall backends.
* `kernel/io/sys_file.c` — file syscall backends mapped to VFS/WFS.
* `user/ntdll/syscall_stubs.asm` — user-mode Nt* call stubs.
* `user/ntdll/include/winn/ntapi.h` — user-visible Nt* declarations.
