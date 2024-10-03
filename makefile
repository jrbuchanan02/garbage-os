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
submake_load = -j12

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

# WARNING: running this target requires that you re-run init!
superclean:
	rm -rf dependencies

define build_arch_binutils = 
	@mkdir -p dependencies/binutils-build-$(1)
	@mkdir -p dependencies/binutils-$(1)
	@cd dependencies/binutils-build-$(1) && ../binutils/configure --target=$(1)-elf --prefix=$(abspath dependencies/binutils-$(1)) --with-sysroot --disable-nls --disable-werror
	@cd dependencies/binutils-build-$(1) && make $(submake_load) && make install $(submake_load) && make distclean $(submake_load)
endef

define build_arch_gcc = 
	@mkdir -p dependencies/gcc-build-$(1)
	@mkdir -p dependencies/gcc-$(1)
	@cd dependencies/gcc-build-$(1) && ../gcc/configure --target=$(1)-elf --prefix=$(abspath dependenices/gcc-$(1)) --disable-nls --enable-languages=c,c++ --without-headers
	@cd dependencies/gcc-build-$(1) && make all-gcc all-target-libgcc install-gcc install-target-libgcc $(submake_load) && make distclean $(submake_load)
endef

define build_arch_kernel_cxx_file = 
	@mkdir -p $(@D)
	@dependencies/gcc-$(1)/$(1)-elf/bin/g++ --std=c++20 -c -ffreestanding -Wall -Wextra -Wpedantic -o $@ $^
endef

define build_arch_kernel_asm_file = 
	@mkdir -p $(@D)
	@dependencies/binutils-$(1)/$(1)-elf/bin/as -c $^ -o $@
endef

define kernel_objects = 
	@build/obj/$(1)/kernel/kernel.o build/obj/$(1)/kernel/asm/$(1)/entry.o build/obj/$(1)kernel/asm/$(1)/halt.o
endef

define link_arch_kernel = 
	@mkdir -p $(@D)
	@dependencies/binutils-$(1)/$(1)-elf-bin/ld -e start -nostdlib -T source/kernel/linker-script/$(1)-script.lds -l dependencies/binutils-$(1)/lib/libgcc -o $@ $^
endef

# TODO: merge nearly identical targets which only differ according to their architecture.
build/bin/i386/garbage.bin: $(call kernel_objects,i386)
	$(call link_arch_kernel,i386)

build/obj/i386/kernel/asm/i386/%.o: source/kernel/asm/i386/%.S
	$(call build_arch_kernel_asm_file,i386)

build/obj/i386/kernel/%.o: source/kernel/%.c++
	$(call build_arch_kernel_cxx_file,i386)

build/bin/riscv32/garbage.bin: $(call kernel_objects,riscv32)
	$(call link_arch_kernel,riscv32)

build/obj/riscv32/kernel/asm/riscv32/%.o: source/kernel/asm/riscv32/%.S
	$(call build_arch_kernel_asm_file,riscv32)

build/obj/riscv32/kernel/%.o: source/kernel/%.c++
	$(call build_arch_kernel_cxx_file,riscv32)

build/bin/riscv64/garbage.bin: $(call kernel_objects,riscv64)
	$(call link_arch_kernel,riscv64)

build/obj/riscv64/kernel/asm/riscv64/%.o: source/kernel/asm/riscv64/%.S
	$(call build_arch_kernel_asm_file,riscv64)

build/obj/riscv64/kernel/%.o: source/kernel/%.c++
	$(call build_arch_kernel_cxx_file,riscv64)

build/bin/x86_64/garbage.bin: $(call kernel_objects,x86_64)
	$(call link_arch_kernel,x86_64)

build/obj/x86_64/kernel/asm/x86_64/%.o: source/kernel/asm/x86_64/%.S
	$(call build_arch_kernel_asm_file,x86_64)

build/obj/x86_64/kernel/%.o: source/kernel/%.c++
	$(call build_arch_kernel_cxx_file,x86_64)

build_i386_binutils:
	$(call build_arch_binutils,i386)
build_i386_gcc: build_i386_binutils
	$(call build_i386_gcc,i386)

build_riscv32_binutils:
	$(call build_arch_binutils,riscv32)
build_riscv32_gcc: build_riscv32_binutils
	$(call build_arch_gcc,riscv32)

build_riscv64_binutils:
	$(call build_arch_binutils,riscv64)
build_riscv64_gcc: build_riscv64_binutils
	$(call build_arch_gcc,riscv64)

build_x86_64_binutils:
	$(call build_arch_binutils,x86_64)
build_x86_64_gcc: build_x86_64_binutils
	$(call build_arch_gcc,x86_64)

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