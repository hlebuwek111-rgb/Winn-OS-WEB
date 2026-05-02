if(NOT WINNOS_ENABLE_BOOT)
  return()
endif()

add_executable(winnos_bootx64
  boot_main.c
)

target_compile_definitions(winnos_bootx64 PRIVATE WINNOS_UEFI_BOOT=1)
set_target_properties(winnos_bootx64 PROPERTIES
  OUTPUT_NAME "BOOTX64"
  SUFFIX ".EFI"
  RUNTIME_OUTPUT_DIRECTORY "${WINNOS_OUTPUT_DIR}/boot/EFI/BOOT"
)
