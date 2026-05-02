cmake_minimum_required(VERSION 3.24)

project(WinnOS LANGUAGES C CXX ASM)

option(WINNOS_BUILD_BOOTLOADER "Build the bootloader target tree" ON)
option(WINNOS_BUILD_KERNEL "Build the kernel target tree" ON)
option(WINNOS_BUILD_DRIVERS "Build the drivers target tree" ON)
option(WINNOS_BUILD_USER "Build the userland target tree" ON)

set(WINNOS_SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(WINNOS_BINARY_ROOT "${CMAKE_CURRENT_BINARY_DIR}")

function(winnos_add_component option_name path label)
    if(${option_name})
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${path}/CMakeLists.txt")
            message(STATUS "Enabling ${label}: ${path}")
            add_subdirectory("${path}")
        else()
            message(STATUS "${label} enabled but ${path}/CMakeLists.txt is missing; skipping for now")
        endif()
    else()
        message(STATUS "${label} disabled")
    endif()
endfunction()

winnos_add_component(WINNOS_BUILD_BOOTLOADER bootloader "bootloader")
winnos_add_component(WINNOS_BUILD_KERNEL kernel "kernel")
winnos_add_component(WINNOS_BUILD_DRIVERS drivers "drivers")
winnos_add_component(WINNOS_BUILD_USER user "user")
