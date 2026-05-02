cmake_minimum_required(VERSION 3.24)

project(WinnOS LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(WINNOS_BUILD_BOOT "Build UEFI boot components" ON)
option(WINNOS_BUILD_KERNEL "Build kernel and HAL components" ON)
option(WINNOS_BUILD_DRIVERS "Build in-tree drivers" ON)
option(WINNOS_BUILD_FS "Build filesystem modules" ON)
option(WINNOS_BUILD_USER "Build user-mode core components" ON)
option(WINNOS_BUILD_SDK "Build/install SDK outputs" ON)
option(WINNOS_BUILD_TOOLS "Build host-side tools" ON)

if(WINNOS_BUILD_BOOT)
  add_subdirectory(boot/uefi)
endif()

if(WINNOS_BUILD_KERNEL)
  add_subdirectory(hal)
  add_subdirectory(kernel)
endif()

if(WINNOS_BUILD_DRIVERS)
  add_subdirectory(drivers)
endif()

if(WINNOS_BUILD_FS)
  add_subdirectory(fs)
endif()

if(WINNOS_BUILD_USER)
  add_subdirectory(user)
endif()

if(WINNOS_BUILD_SDK)
  add_subdirectory(sdk)
endif()

if(WINNOS_BUILD_TOOLS)
  add_subdirectory(tools)
endif()
