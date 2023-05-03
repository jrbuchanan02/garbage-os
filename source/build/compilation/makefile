################################################################################
# Garbage OS's Makefile. 													   #
# Author: Joshua Buchanan (joshuarobertbuchanan@gmail.com)					   #
# Brief: Builds Garbage OS and runs its unittests.							   #
# Date: 2023-04-XX															   #
# Version: 1																   #
# Copyright: This file is part of Garbage OS. Garbage OS is Copyright (C) 2023 #
# Joshau Buchanan and published under the LGPL version 2.1. You are free to    #
# use this source code in your project even if your code is not open source.   #
################################################################################

# these variables below describe common flags used by the compiler in multiple
# locations.
common_gcc_flags = --std=c++20 -Wall -Wextra -Wpedantic -Werror
common_include = -I source

native_compiler = g++
x86_compiler = i686-linux-gnu-g++
aarch64_compiler = aarch64-linux-gnu-g++-11

os_code_flags = -nostdlib -fzero-call-used-regs=all -ffreestanding -nodefaultlibs -znocombreloc -Wl,-Bsymbolic -fno-exceptions -fno-rtti
x86_64_flags = -m64 -mno-red-zone
ia32_flags = -m32
aarch64_flags = -march=armv8-a

define generator_build
@echo Running Build Script $@
@python3 $@
@echo $@ was Run.
endef

define test_build
@echo Building target $@
@echo Target $@ requires files $^
@echo Target $@ will build with the C++ source files $(filter %.c++,$^)
@mkdir -p $(dir $@)
@g++ -o $@ $(common_include) -I ./ -I test $(common_gcc_flags) $(filter %.c++,$^)
@echo Built Target $@
endef

define loader_x86_64_build
@mkdir -p $(dir $@)
@echo building $@
@$(x86_compiler) $(filter %.c++,$^) -c $(common_gcc_flags) $(common_include) $(os_code_flags) $(x86_64_flags) -o $@ -DANY_86_TARGET -D_64_BITS
@echo compiled $@
endef

define loader_ia32_build
@mkdir -p $(dir $@)
@echo building $@
@$(x86_compiler) $(filter %.c++,$^) -c $(common_gcc_flags) $(common_include) $(os_code_flags) $(ia32_flags) -o $@ -DANY_86_TARGET -D_32_BITS
@echo compiled $@
endef

define loader_aarch64_build
@mkdir -p $(dir $@)
@echo building $@
@$(aarch64_compiler) $(filter %.c++,$^) -c $(common_gcc_flags) $(common_include) $(os_code_flags) $(aarch64_flags) -o $@ -D_64_BITS
@echo compiled $@
endef

source/build/generation/crc32_test.py:
	$(generator_build)

generator_files = $(wildcard source/build/generation/*.py)

# auto-generated files. Anything that should be in the generated folder
# will have been built from a generator file.

build/generated/crc32results.h++: source/build/generation/crc32_test.py

build/test/utility_functions_test: test/utility_functions.c++ build/generated/crc32results.h++
	$(test_build)
build/test/check_valid_pe32+: test/check_valid_pe32+.c++
	$(test_build)

# the various tests
tests = build/test/utility_functions_test build/test/check_valid_pe32+

build/obj/loader/environment/x86_64_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_x86_64_build)
build/obj/loader/setup/x86_64_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_x86_64_build)

build/obj/loader/environment/ia32_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_ia32_build)
build/obj/loader/setup/ia32_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_ia32_build)

build/obj/loader/environment/aarch64_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_aarch64_build)
build/obj/loader/setup/aarch64_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_aarch64_build)

build/bin/BOOTX64.EFI: build/obj/loader/environment/x86_64_efi_main.o build/obj/loader/setup/x86_64_loader_main.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(x86_compiler) $^ $(common_gcc_flags) $(common_include) $(os_code_flags) $(x86_64_flags) -e efi_main -o $@ -Wl,-T,source/build/link/pe32p_x64.ld
	@echo $@ linked.
build/bin/BOOTIA32.BIN: build/obj/loader/environment/ia32_efi_main.o build/obj/loader/setup/ia32_loader_main.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(x86_compiler) $^ $(common_gcc_flags) $(common_include) $(os_code_flags) $(ia32_flags) -e efi_main -o $@
	@echo $@ linked.
build/bin/BOOTAA64.BIN: build/obj/loader/environment/aarch64_efi_main.o build/obj/loader/setup/aarch64_loader_main.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(aarch64_compiler) $^ $(common_gcc_flags) $(common_include) $(os_code_flags) $(aarch64_flags) -e efi_main -o $@

#build/bin/BOOTX64.EFI: build/bin/BOOTX64.BIN
#	@echo Copying the ELF-formatted binary $^ into the PE-COFF formatted binary $@
#	@i686-linux-gnu-objcopy -I elf64-x86-64 -O pei-x86-64 $^ $@

build/bin/BOOTIA32.EFI: build/bin/BOOTIA32.BIN
	@echo Copying the ELF-formatted binary $^ into the PE-COFF formatted binary $@
	@i686-linux-gnu-objcopy -I elf32-i386 -O pei-i386 $^ $@

build/bin/BOOTAA64.EFI: build/bin/BOOTAA64.BIN
	@echo Copying the ELF-formatted binary $^ into the PE-COFF formatted binary $@
	@aarch64-linux-gnu-objcopy -I elf64-littleaarch64 -O pei-aarch64-little $^ $@


x86_64_loader: build/bin/BOOTX64.EFI
ia32_loader: build/bin/BOOTIA32.EFI
aarch64_loader: build/bin/BOOTAA64.EFI

loaders: x86_64_loader ia32_loader aarch64_loader

run_crc32_test: build/test/utility_functions_test
	@./$^

# addtional file here is actual dummy output from link.exe for an x64 file targeting
# efi.
run_check_valid_pe32+_test: build/test/check_valid_pe32+ build/bin/BOOTX64.EFI build/bin/BOOTIA32.EFI build/bin/BOOTAA64.EFI
	@./$(filter-out %.c++,$^) $(filter %.c++,$^) build/bin/BOOTAUTH.EFI

run_tests: run_crc32_test run_check_valid_pe32+_test

all: $(generator_files) $(tests) loaders run_tests
	@echo Generator Files: $(generator_files)
	@echo Built Tests: $(tests)