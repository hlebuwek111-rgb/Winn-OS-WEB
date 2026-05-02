cmake_minimum_required(VERSION 3.24)
project(WinnOSNative C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(kernel)
add_subdirectory(drivers/serial)
add_subdirectory(fs/wfs)
add_subdirectory(user/shell)
