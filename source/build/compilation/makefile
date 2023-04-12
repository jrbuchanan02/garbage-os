################################################################################
# Garbage OS's Makefile. 													   #
#																			   #
################################################################################

## defines
common_gcc_flags = --std=c++20 -Wall -Wextra -Wpedantic -Werror
common_include = -I source

os_code_flags = -nostdlib -fzero-call-used-regs=all -ffreestanding

x86_64_flags = -m64 -mno-red-zone
ia32_flags = -m32

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
@i686-linux-gnu-g++ $^ -c $(common_gcc_flags) $(common_include) $(os_code_flags) $(x86_64_flags) -o $@ -DANY_86_TARGET
@echo compiled $@
endef

define loader_ia32_build
@mkdir -p $(dir $@)
@echo building $@
@i686-linux-gnu-g++ $^ -c $(common_gcc_flags) $(common_include) $(os_code_flags) $(ia32_flags) -o $@ -DANY_86_TARGET
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

# the various tests
tests = build/test/utility_functions_test

build/obj/loader/environment/x86_64_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_x86_64_build)
build/obj/loader/setup/x86_64_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_x86_64_build)

build/obj/loader/environment/ia32_efi_main.o: source/loader/environment/efi_main.c++
	$(loader_ia32_build)
build/obj/loader/setup/ia32_loader_main.o: source/loader/setup/loader_main.c++
	$(loader_ia32_build)

build/bin/BOOTX64.BIN: build/obj/loader/environment/x86_64_efi_main.o build/obj/loader/setup/x86_64_loader_main.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@i686-linux-gnu-g++ $^ $(common_gcc_flags) $(common_include) $(os_code_flags) $(x86_64_flags) -e efi_main -o ./build/bin/BOOTX64.BIN
	@echo $@ linked.
build/bin/BOOTIA32.BIN: build/obj/loader/environment/ia32_efi_main.o build/obj/loader/setup/ia32_loader_main.o
	@echo Linking $@
	@mkdir -p $(dir $@)
	@i686-linux-gnu-g++ $^ $(common_gcc_flags) $(common_include) $(os_code_flags) $(ia32_flags) -e efi_main -o ./build/bin/BOOTIA32.BIN
	@echo $@ linked.

x86_64_loader: build/bin/BOOTX64.BIN
ia32_loader: build/bin/BOOTIA32.BIN

all: $(generator_files) $(tests) x86_64_loader ia32_loader
	@echo Generator Files: $(generator_files)
	@echo Built Tests: $(tests)