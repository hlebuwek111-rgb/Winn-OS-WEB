OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(kernel_entry64)

KERNEL_BASE = 0x100000;

SECTIONS {
  . = KERNEL_BASE;

  .text : ALIGN(0x1000) {
    *(.text*)
  }

  .rodata : ALIGN(0x1000) {
    *(.rodata*)
  }

  .data : ALIGN(0x1000) {
    *(.data*)
  }

  .bss : ALIGN(0x1000) {
    *(COMMON)
    *(.bss*)
  }
}
