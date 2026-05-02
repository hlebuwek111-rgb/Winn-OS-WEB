# Syscall Surface

## Overview
This document describes the initial system call interface strategy, focusing on an Nt*-style API, boundary validation, and user/kernel isolation expectations.

## User/Kernel Boundary
Key boundary requirements:
- Syscalls are the only supported transition path from user mode to kernel services.
- All user pointers are treated as untrusted and validated/probed before dereference.
- Capability/handle checks are performed in kernel context.
- Return values use stable status code semantics and explicit output lengths.

ABI considerations:
- Architecture-specific syscall instruction and register convention are documented in a shared ABI header.
- Versioning strategy permits additive extension without breaking existing binaries.

## Initial Nt* API Surface (Planned)
Process/thread:
- `NtProcessCreate`
- `NtProcessExit`
- `NtThreadCreate`
- `NtThreadExit`
- `NtThreadSleep`

Virtual memory:
- `NtVirtualAlloc`
- `NtVirtualFree`
- `NtVirtualProtect`
- `NtMapView`
- `NtUnmapView`

Object/handle:
- `NtHandleClose`
- `NtDuplicateHandle`
- `NtQueryObject`

File/IO:
- `NtFileOpen`
- `NtFileCreate`
- `NtFileRead`
- `NtFileWrite`
- `NtFileClose`
- `NtFileQueryInfo`

System:
- `NtSystemInfo`
- `NtTimeQuery`
- `NtDebugLog`

## Dispatch and Validation Model
1. User mode issues syscall with number + argument block/register payload.
2. Entry stub saves context and switches to trusted kernel stack.
3. Dispatcher resolves syscall ID to handler table.
4. Marshalling layer validates/copies arguments.
5. Handler executes with object manager / VM / VFS services.
6. Status and outputs are copied back to user buffers with bounds checks.

## Security Expectations
- Reject unknown syscall IDs.
- Enforce least-privilege access checks per object type.
- Audit sensitive operations (process creation, mapping executable pages, raw device IO).
- Keep per-syscall argument schemas explicit to reduce confused-deputy risks.

## Planned Source Files
- `kernel/arch/x86_64/syscall_entry.S` — Low-level syscall entry/exit path and register save frame.
- `kernel/syscall/dispatch.c` (`sys_dispatch`) — Syscall table lookup and dispatch core.
- `kernel/syscall/probe.c` (`probe_user_read`, `probe_user_write`) — User pointer validation helpers.
- `kernel/syscall/nt_process.c` (`NtProcessCreate`, `NtThreadCreate`) — Process/thread syscall handlers.
- `kernel/syscall/nt_vm.c` (`NtVirtualAlloc`, `NtVirtualProtect`) — Virtual memory syscall handlers.
- `kernel/syscall/nt_file.c` (`NtFileOpen`, `NtFileRead`, `NtFileWrite`) — File and IO syscall handlers.
- `kernel/include/wos/syscall_abi.h` — Shared syscall numbers, calling convention, and status codes.
- `user/ntdll/syscall_stubs.S` — User-mode syscall stubs matching kernel ABI.
