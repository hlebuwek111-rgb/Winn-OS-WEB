.section .text
.global kernel_entry64
.extern kernel_main

kernel_entry64:
    cli
    lea kernel_stack_top(%rip), %rsp
    andq $-16, %rsp

    mov %rdi, %rdi
    call kernel_main

.hang:
    hlt
    jmp .hang

.section .bss
.align 16
kernel_stack:
    .skip 32768
kernel_stack_top:
