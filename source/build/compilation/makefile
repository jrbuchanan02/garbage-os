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

# set some global variables to useful default values
CXX ?= g++
CC  ?= gcc

# these variables below describe common flags used by the compiler in multiple
# locations.
cxx_standard = c++20
warnings = all extra pedantic error
includes = source

kernel_overrides  = freestanding zero-call-used-regs=all no-exceptions no-rtti
linker_overrides  = nocombreloc
compiler_requests = nostdlib nodefaultlibs
linker_requests   = -Bsymbolic

8664_specializations = 64 no-red-zone
ia32_specializations = 32
aa64_specializations = arch=armv8-a

32_bit_defines = _32_BITS
64_bit_defines = _64_BITS

x86_defines = ANY_86_TARGET
arm_defines = ANY_ARM_TARGET

8664_defines = $(64_bit_defines) $(x86_defines)
ia32_defines = $(32_bit_defines) $(x86_defines)
aa64_defines = $(64_bit_defines) $(arm_defines)

8664_define_flags = $(8664_defines:%=-D%)
ia32_define_flags = $(ia32_defines:%=-D%)
aa64_define_flags = $(aa64_defines:%=-D%)

cxx_std_flag = --std=$(cxx_standard)
warning_flag = $(warnings:%=-W%)
include_flag = $(includes:%=-I %)

kernel_override_flag  = $(kernel_overrides:%=-f%)
linker_override_flag  = $(linker_overrides:%=-z%)
compiler_request_flag = $(compiler_requests:%=-%)
linker_request_flag   = $(linker_requests:%=-Wl,%)

8664_specialization_flag = $(8664_specializations:%=-m%)
ia32_specialization_flag = $(ia32_specializations:%=-m%)
aa64_specialization_flag = $(aa64_specializations:%=-m%)

common_code_flags    = $(cxx_std_flag) $(warning_flag)
common_include_flags = $(include_flag)

common_flags = $(common_code_flags) $(common_include_flags)

default_native_compiler = $(CXX)
default_x86_compiler    = i686-linux-gnu-g++
default_aa64_compiler   = aarch64-linux-gnu-g++

native_compiler  ?= $(default_native_compiler)
x86_compiler     ?= $(default_x86_compiler)
aarch64_compiler ?= $(default_aa64_compiler)

kernel_flags    = $(kernel_override_flag) $(linker_override_flag) $(compiler_request_flag) $(linker_request_flag)
8664_code_flags = $(8664_specialization_flag) $(8664_define_flags)
ia32_code_flags = $(ia32_specialization_flag) $(ia32_define_flags)
aa64_code_flags = $(aa64_specialization_flag) $(aa64_define_flags)

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
@$(native_compiler) $(common_flags) --coverage -I ./ -I test $(filter %.c++,$^) -o $@
@echo Running Coverage on $@
@gcovr $@ -r ./ --html-details ./docs/coverage/$(@F).html
@echo Built Target $@
endef

define loader_x86_64_build
@mkdir -p $(dir $@)
@echo building $@
$(x86_compiler) $(filter %.c++,$^) -c $(common_flags) $(kernel_flags) $(8664_code_flags) -o $@
@echo compiled $@
endef

define loader_ia32_build
@mkdir -p $(dir $@)
@echo building $@
@$(x86_compiler) $(filter %.c++,$^) -c $(common_flags) $(kernel_flags) $(ia32_code_flags) -o $@
@echo compiled $@
endef

define loader_aarch64_build
@mkdir -p $(dir $@)
@echo building $@
@$(aarch64_compiler) $(filter %.c++,$^) -c $(common_flags) $(kernel_flags) $(aa64_code_flags) -o $@
@echo compiled $@
endef

define build_this
$(1): $(2)
	$$(3)
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

#build/obj/loader/environment/x86_64_efi_main.o: source/loader/environment/efi_main.c++
#	$(loader_x86_64_build)
$(eval $(call build_this build/obj/loader/setup/x86_64_loader_main.o,loader_x86_64_build,source/loader/setup/loader_main.c++))
$(eval $(call build_this build/obj/loader/setup/ia32_loader_main.o,loader_ia32_build,source/loader/setup/loader_main.c++))
$(eval $(call build_this build/obj/loader/setup/aarch64_loader_main.o,loader_aarch64_build,source/loader/setup/loader_main.c++))

build/obj/loader/environment/ia32_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_ia32_build)
build/obj/loader/setup/ia32_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_ia32_build)
build/obj/external_standards/uefi/ia32_added_functionality.o: source/external_standards/uefi/added_functionality.c++
	$(loader_ia32_build)

build/obj/loader/environment/aarch64_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_aarch64_build)
build/obj/loader/setup/aarch64_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_aarch64_build)
build/obj/external_standards/uefi/aarch64_added_functionality.o: source/external_standards/uefi/added_functionality.c++
	$(loader_aarch64_build)

build/bin/BOOTX64.EFI: build/obj/loader/environment/x86_64_efi_main.o build/obj/loader/setup/x86_64_loader_main.o build/obj/external_standards/uefi/x86_64_added_functionality.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(x86_compiler) $^ $(common_flags) $(kernel_flags) $(8664_code_flags) -e efi_main -o $@ -Wl,-T,source/build/link/pe32p_x64.ld
	@echo $@ linked.
build/bin/BOOTIA32.BIN: build/obj/loader/environment/ia32_efi_main.o build/obj/loader/setup/ia32_loader_main.o build/obj/external_standards/uefi/ia32_added_functionality.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(x86_compiler) $^ $(common_flags) $(kernel_flags) $(ia32_code_flags) -e efi_main -o $@
	@echo $@ linked.
build/bin/BOOTAA64.BIN: build/obj/loader/environment/aarch64_efi_main.o build/obj/loader/setup/aarch64_loader_main.o build/obj/external_standards/uefi/aarch64_added_functionality.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(aarch64_compiler) $^ $(common_flags) $(kernel_flags) $(aarch64_code_flags) -e efi_main -o $@

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
	@./$(filter-out %.c++,$^) $(filter %.c++,$^) testfiles/BOOTTEST.EFI

run_tests: run_crc32_test run_check_valid_pe32+_test

all: $(generator_files) $(tests) loaders run_tests
	@echo Generator Files: $(generator_files)
	@echo Built Tests: $(tests)