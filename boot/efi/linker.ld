OUTPUT_FORMAT("pei-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(efi_main)

SECTIONS {
  . = 0;
  .text : { *(.text*) }
  .rdata : { *(.rdata*) *(.rodata*) }
  .data : { *(.data*) }
  .pdata : { *(.pdata*) }
  .xdata : { *(.xdata*) }
  .bss : { *(.bss*) *(COMMON) }
}
