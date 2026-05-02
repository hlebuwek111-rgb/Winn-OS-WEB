add_library(winos_kernel STATIC
  ke_main.c
  arch/x86_64/entry_stub.S
  arch/x86_64/gdt.c
  arch/x86_64/idt.c
  arch/x86_64/pmm.c
  arch/x86_64/vmm.c
  arch/x86_64/scheduler.c
)

target_include_directories(winos_kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
