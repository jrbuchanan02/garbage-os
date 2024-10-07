#################################################################################
#							Garbage OS Makefile									#
#																				#
#	This file describes the build process for a Garbage OS ISO image and allows #
#	for some basic testing of said image. 										#
#																				#
#																				#
#	Q:	Why a Makefile:															#
#		A:	Makefiles give more control over this build process and still 		#
#			eliminate the need for YOU to remember all the commands and to 		#
#			build each and every file for Garbage OS!							#
#################################################################################

# TODO: change get_gcc and get_binutils to check if the repository already exists

# on recursive calls to make (i.e. building dependencies), make the average load this much.
# feel free to change this value to a number of threads you are comfortable with using.

submake_load = -j16

create_dependencies_dir:
	@mkdir -p dependencies

# Build GCC version 14.2 from source, start by getting the repository
get_gcc: create_dependencies_dir
	@mkdir -p dependencies/gcc
	git clone git://gcc.gnu.org/git/gcc.git dependencies/gcc
	cd dependencies/gcc && git checkout releases/gcc-14
# Build Binutils version 2.43 from source, start by getting the repository
get_binutils: create_dependencies_dir
	@mkdir -p dependencies/binutils
	git clone git://sourceware.org/git/binutils-gdb.git dependencies/binutils
	cd dependencies/binutils && git checkout binutils-2_43

init: get_gcc get_binutils

clean:
	rm -rf build

# clear out the CXX, CXXFLAGS, ASM, and LD environment variables such that
# if there is an issue with conditionally assigning the pattern-dependent
# versions of these variables, then compilation will fail
common_cxxflags = --std=c++20 -fno-exceptions -fno-rtti -ffreestanding -Wall -Wextra -Wpedantic -O1 -S -I source
i386_cxx_flags = $(common_cxxflags) -m32 -mno-80387 -march=i386 -mtune=generic
riscv32_cxx_flags = $(common_cxxflags) -mabi=ilp32 -march=rv32izicsr -mlittle-endian
riscv64_cxx_flags = $(common_cxxflags) -mabi=lp64 -march=rv64izicsr -mlittle-endian
x86_64_cxx_flags = $(common_cxxflags) -m64 -march=x86-64 -mtune=generic -mno-red-zone

common_ld_flags = -e start -nostdlib

architectures = i386 riscv32 riscv64

# WARNING: running this target requires that you re-run init!
superclean:
	rm -rf dependencies

define build_arch_binutils = 
	@mkdir -p dependencies/binutils-build-$(1)
	@mkdir -p dependencies/binutils-$(1)
	@cd dependencies/binutils-build-$(1) && ../binutils/configure --target=$(1)-elf --prefix=$(abspath dependencies/binutils-$(1)) --with-sysroot --disable-nls --disable-werror
	@cd dependencies/binutils-build-$(1) && make $(submake_load) && make install $(submake_load)
endef

define build_arch_gcc = 
	@mkdir -p dependencies/gcc-build-$(1)
	@mkdir -p dependencies/gcc-$(1)
	@cd dependencies/gcc-build-$(1) && ../gcc/configure --target=$(1)-linux-gnu --prefix=$(abspath dependencies/gcc-$(1)) --disable-nls --enable-languages=c,c++ --without-headers --disable-multilib
	@cd dependencies/gcc-build-$(1) && make all-gcc $(submake_load) 
	@cd dependencies/gcc-build-$(1) && make install-gcc $(submake_load)
endef

build/asm/$(ARCH)/%.c++.S: source/%.c++
	@mkdir -p $(@D)
	@dependencies/gcc-$(ARCH)/bin/$(ARCH)-linux-gnu-g++ $($(ARCH)_cxx_flags) -o $@ $^
build/asm/$(ARCH)/%.asm.S: source/$(ARCH)/%.S
	@mkdir -p $(@D)
	@cp $^ $@

build/obj/$(ARCH)/%.o: build/asm/$(ARCH)/%.S
	@mkdir -p $(@D)
	@dependencies/binutils-$(ARCH)/bin/$(ARCH)-elf-as -o $@ $^

build/bin/$(ARCH)/garbage.bin: build/obj/$(ARCH)/kernel/kmain.c++.o build/obj/$(ARCH)/kernel/entry.asm.o build/obj/$(ARCH)/kernel/halt.asm.o
	@mkdir -p $(@D)
	@dependencies/binutils-$(ARCH)/bin/$(ARCH)-elf-ld $(common_ld_flags) -T source/$(ARCH)/garbage.bin.lds -o $@ $^

build_arch_kernel: build/bin/$(ARCH)/garbage.bin

build_kernels:
# the @echo "" ensures that the line does not end in an &&, which is illegal for the shell
	$(foreach arch,$(architectures),$(MAKE) $(MAKEFLAGS) -e ARCH=$(arch) build_arch_kernel &&) echo ""

.PHONY: build_i386_binutils build_i386_gcc build_riscv32_binutils build_riscv32_gcc build_riscv64_binutils build_riscv64_gcc build_x86_64_binutils build_x86_64_gcc

# use this target to build the toolchain since you probably want to build
# multithreaded toolchains and there (probably) will be issues if you have GCC and/or
# binutils building at the same time
build_toolchains:
	$(MAKE) build_i386_binutils
	$(MAKE) build_riscv32_binutils
	$(MAKE) build_riscv64_binutils
	$(MAKE) build_x86_64_binutils
	$(MAKE) build_i386_gcc
	$(MAKE) build_riscv32_gcc
	$(MAKE) build_riscv64_gcc
	$(MAKE) build_x86_64_gcc
# use this target to build the compilers
build_compilers:
	$(MAKE) build_i386_gcc
	$(MAKE) build_riscv32_gcc
	$(MAKE) build_riscv64_gcc
	$(MAKE) build_x86_64_gcc

# use this target to build the toolchain since you probably want to build
# multithreaded toolchains and there (probably) will be issues if you have GCC and/or
# binutils building at the same time
build_toolchains:
	$(MAKE) build_i386_gcc
	$(MAKE) build_riscv32_gcc
	$(MAKE) build_riscv64_gcc
	$(MAKE) build_x86_64_gcc

# target that builds the kernel
build_kernel: build/bin/i386/garbage.bin build/bin/riscv32/garbage.bin build/bin/riscv64/garbage.bin build/bin/x86_64/garbage.bin

test_i386_kernel: build/bin/i386/garbage.bin
	qemu-system-i386 -cpu x86 486 -kernel build/bin/i386/garbage.bin
test_riscv32_kernel: build/bin/riscv32/garbage.bin
	qemu-system-riscv32 -cpu rv32 -kernel build/bin/riscv32/garbage.bin
test_riscv64_kernel: build/bin/riscv64/garbage.bin
	qemu-system-riscv64 -cpu rv64 -kernel build/bin/riscv64/garbage.bin
test_x86_64_kernel: build/bin/x86_64/garbage.bin
	qemu-system-x86_64 -cpu x86_64 -kernel build/bin/x86_64/garbage.bin