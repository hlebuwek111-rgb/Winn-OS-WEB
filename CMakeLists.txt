cmake_minimum_required(VERSION 3.24)

project(WinnOS LANGUAGES C CXX ASM_NASM)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain-winnos.cmake" CACHE FILEPATH "WinnOS toolchain")

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  message(FATAL_ERROR "WinnOS currently targets x86_64 only.")
endif()

set(WINNOS_TRIPLE "x86_64-pc-winnos" CACHE STRING "Target triple for cross-compilation")
set(WINNOS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/artifacts")
file(MAKE_DIRECTORY "${WINNOS_OUTPUT_DIR}")

option(WINNOS_ENABLE_BOOT "Build UEFI bootloader" ON)
option(WINNOS_ENABLE_KERNEL "Build kernel image" ON)
option(WINNOS_ENABLE_DRIVERS "Build driver stubs" ON)

add_subdirectory(boot)
add_subdirectory(kernel)
add_subdirectory(hal)
add_subdirectory(drivers)
add_subdirectory(fs/wfs)
add_subdirectory(user)
add_subdirectory(tools)

message(STATUS "WinnOS target triple: ${WINNOS_TRIPLE}")
message(STATUS "Artifacts output dir: ${WINNOS_OUTPUT_DIR}")
