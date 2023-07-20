cmake_minimum_required(VERSION 3.22)
project(Garbage_OS VERSION 1 LANGUAGES CXX ASM)

# global compiler options. Note that this command runs in build, so it needs
# to refer relatively to the repository root.
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/compiler_dispatcher.sh)

add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-Wextra)
add_compile_options(-Wpedantic)
add_compile_options(--std=c++20)

# global include directories
include_directories(./source ./)

# tests
add_executable(utility_functions_test)
add_executable(check_valid_pe32+_test)

# UEFI binaries
add_executable(BOOTIA32.EFI)
add_executable(BOOTX64.EFI)
add_executable(BOOTAA64.EFI)

# Other Loaders' binaries go here.

# Kernel Binaries go here.

# API Binaries go here.

# Unification Layer Binaries go here.

# Builtin Appications' Binaries go here.

# Adding sources to targets

target_sources(utility_functions_test PRIVATE test/utility_functions.c++)

target_sources(check_valid_pe32+_test PRIVATE test/check_valid_pe32+.c++)

set(LOADER_SOURCES PRIVATE source/external_standards/uefi/added_functionality.c++
                   PRIVATE source/loader/environment/efi_main.c++
                   PRIVATE source/loader/setup/loader_main.c++)

target_sources(BOOTIA32.EFI ${LOADER_SOURCES})
target_sources(BOOTX64.EFI  ${LOADER_SOURCES})
target_sources(BOOTAA64.EFI ${LOADER_SOURCES})

# Adding more specialized compiler commands to targets.

target_compile_options(utility_functions_test PRIVATE -fprofile-arcs 
                                              PRIVATE -ftest-coverage 
                                              PRIVATE -g
                                              PRIVATE -arch=native)
target_compile_options(check_valid_pe32+_test PRIVATE -fprofile-arcs 
                                              PRIVATE -ftest-coverage 
                                              PRIVATE -g
                                              PRIVATE -arch=native)

target_link_libraries(utility_functions_test gcov)
target_link_libraries(check_valid_pe32+_test gcov)

target_link_options(utility_functions_test PRIVATE --coverage
                                           PRIVATE -arch=native)
target_link_options(check_valid_pe32+_test PRIVATE --coverage
                                           PRIVATE -arch=native)

# *compiler* loader options
set(LOADER_OPTIONS PRIVATE -ffreestanding 
                   PRIVATE -fzero-call-used-regs=all 
                   PRIVATE -fno-exceptions
                   PRIVATE -fno-rtti 
                   PRIVATE -nostdlib 
                   PRIVATE -nodefaultlibs
                   PRIVATE -znocombreloc
                   PRIVATE -fno-pic)

target_compile_options(BOOTIA32.EFI ${LOADER_OPTIONS})
target_compile_options(BOOTX64.EFI  ${LOADER_OPTIONS})
target_compile_options(BOOTAA64.EFI ${LOADER_OPTIONS})

target_compile_options(BOOTIA32.EFI PRIVATE -m32
                                    PRIVATE -arch=ia32)
target_compile_options(BOOTX64.EFI  PRIVATE -m64 
                                    PRIVATE -mno-red-zone
                                    PRIVATE -arch=8664)
target_compile_options(BOOTAA64.EFI PRIVATE -arch=aa64)

target_compile_definitions(BOOTIA32.EFI PRIVATE _32_BITS
                                        PRIVATE ANY_86_TARGET)
target_compile_definitions(BOOTX64.EFI  PRIVATE _64_BITS
                                        PRIVATE ANY_86_TARGET)
target_compile_definitions(BOOTAA64.EFI PRIVATE _64_BITS
                                        PRIVATE ANY_ARM_TARGET)

# TODO: update these linker scripts to all match the correct architectures.
#       until this is done, the check_valid_pe32+ test will fail.
target_link_options(BOOTIA32.EFI PRIVATE -T ../source/build/link/pe32p_x64.ld
                                 PRIVATE -arch=ia32
                                 PRIVATE -e efi_main
                                 PRIVATE -Bsymbolic
                                 PRIVATE -m32
                                 PRIVATE -Wl,--oformat,binary)
target_link_options(BOOTIA32.EFI ${LOADER_OPTIONS})
target_link_options(BOOTX64.EFI  PRIVATE -T ../source/build/link/pe32p_x64.ld
                                 PRIVATE -arch=8664
                                 PRIVATE -e efi_main
                                 PRIVATE -Bsymbolic
                                 PRIVATE -m64
                                 PRIVATE -Wl,--oformat,binary)
target_link_options(BOOTX64.EFI ${LOADER_OPTIONS})
# mysteriously fails!?
target_link_options(BOOTAA64.EFI PRIVATE -T ../source/build/link/pe32p_x64.ld
                                 PRIVATE -arch=aa64
                                 PRIVATE -e efi_main
                                 PRIVATE -Bsymbolic
                                 PRIVATE -Wl,--oformat,pei-aarch64-little)
target_link_options(BOOTAA64.EFI ${LOADER_OPTIONS})

