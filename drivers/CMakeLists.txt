if(NOT WINNOS_ENABLE_DRIVERS)
  return()
endif()

add_library(driver_stub_network STATIC network_stub.c)
add_library(driver_stub_storage STATIC storage_stub.c)

set_target_properties(driver_stub_network driver_stub_storage PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${WINNOS_OUTPUT_DIR}/drivers"
)
