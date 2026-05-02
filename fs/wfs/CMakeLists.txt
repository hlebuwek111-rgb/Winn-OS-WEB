add_library(winos_wfs STATIC wfs_format.c wfs_driver.c)
target_include_directories(winos_wfs PUBLIC ${CMAKE_SOURCE_DIR}/kernel/include)
