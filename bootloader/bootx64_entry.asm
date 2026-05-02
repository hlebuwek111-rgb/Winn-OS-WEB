; Winn OS UEFI bootloader entry (NASM syntax)
BITS 64
DEFAULT REL
GLOBAL efi_main

SECTION .text
efi_main:
    ; RCX=ImageHandle, RDX=SystemTable in Microsoft x64 EFI ABI
    push rbp
    mov rbp, rsp

    ; TODO: Acquire GetMemoryMap, open WFS volume, load \System32\winoskrnl.exe
    ; TODO: Build boot parameter block and jump to kernel entry.

    xor eax, eax ; EFI_SUCCESS
    leave
    ret
