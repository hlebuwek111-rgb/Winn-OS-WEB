.section .text
.global winos_kernel_entry
.type winos_kernel_entry, @function
winos_kernel_entry:
    push %rbp
    mov %rsp, %rbp
    andq $~0xF, %rsp

    mov %rdi, %rdi
    call ke_main

1:
    hlt
    jmp 1b
