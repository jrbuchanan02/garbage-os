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
submake_load = -l 0.75

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
	mkdir -p dependencies/binutils-build-$(1)
	mkdir -p dependencies/binutils-$(1)
	cd dependencies/binutils-build-$(1) && ../binutils/configure --target=$(1)-elf --prefix=$(abspath dependencies/binutils-$(1)) --with-sysroot --disable-nls --disable-werror
	cd dependencies/binutils-build-$(1) && make $(submake_load) && make install $(submake_load)
endef

define build_arch_gcc = 
	mkdir -p dependencies/gcc-build-$(1)
	mkdir -p dependencies/gcc-$(1)
	cd dependencies/gcc-build-$(1) && ../gcc/configure --target=$(1)-elf --prefix=$(abspath dependenices/gcc-$(1)) --disable-nls --enable-languages=c,c++ --without-headers
	cd dependencies/gcc-build-$(1) && make all-gcc all-target-libgcc install-gcc install-target-libgcc $(submake_load)
endef

build_x86_64_binutils:
	$(call build_arch_binutils,x86_64)

build_x86_64_gcc: build_x86_64_binutils
	$(call build_arch_gcc,x86_64)

