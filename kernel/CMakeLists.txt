if(NOT WINNOS_ENABLE_KERNEL)
  return()
endif()

add_executable(winnos_kernel
  kernel_main.c
)

set_target_properties(winnos_kernel PROPERTIES
  OUTPUT_NAME "winnoskrnl"
  SUFFIX ".exe"
  RUNTIME_OUTPUT_DIRECTORY "${WINNOS_OUTPUT_DIR}/kernel"
)
