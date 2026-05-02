# Cross toolchain for WinnOS x86_64 PC firmware targets.
# Intended for Clang + LLD producing PE/COFF artifacts.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(WINNOS_TRIPLE x86_64-pc-windows-msvc CACHE STRING "Target triple for WinnOS toolchain")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_LINKER lld-link)
set(CMAKE_AR llvm-ar)
set(CMAKE_RANLIB llvm-ranlib)
set(CMAKE_OBJCOPY llvm-objcopy)
set(CMAKE_STRIP llvm-strip)
set(CMAKE_NM llvm-nm)

set(CMAKE_C_COMPILER_TARGET ${WINNOS_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${WINNOS_TRIPLE})
set(CMAKE_ASM_COMPILER_TARGET ${WINNOS_TRIPLE})

set(WINNOS_COMMON_FLAGS "-ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone")

set(CMAKE_C_FLAGS_INIT "${WINNOS_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${WINNOS_COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${WINNOS_COMMON_FLAGS}")

set(WINNOS_LINK_FLAGS "/nologo /subsystem:efi_application /entry:EfiMain /nodefaultlib /machine:x64")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${WINNOS_LINK_FLAGS}")

# CMake's executable suffix for PE images.
set(CMAKE_EXECUTABLE_SUFFIX ".efi")
