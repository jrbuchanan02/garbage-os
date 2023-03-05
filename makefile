################################################################################
# The "entry point" for the build process and any other operation which uses   #
# the make commandline tool. 												   #
# 																			   #
# Author: Joshua Buchanan (@jrbuchanan02)									   #
# Created: 2021-xx-xx (I don't know when?)									   #
# Modified: 2022-03-22														   #
# Version: 2																   #
#																		       # 
# This file allows the recursive and reflective building of all source files   #
# within Garbage OS. This process is recursive in that each directory 		   #
# containing source files has a makefile within it which builds that directory #
# and calls makefiles in its subdirectories. It is also reflective in that it  #
# uses any reflection-features present in make to find all necessary files.    #
#																		       #
# The build process:														   #
# - Installation:															   #
#   In the installation phase of the build process, we need to clone and then  #
# build GCC, G++, and GAS for the native host and all supported targets by     #
# GOS. This requires:														   #
# 		+ A list of all supported targets									   #
# 		+ The URL to clone GCC, G++, GAS from								   #
################################################################################

#global variables.
# mkdir command. On Mingw, this command would be different and have to likely
# specify the entire path.
MKDIR = mkdir
# GCC's version number, 
GMV = 11
GCC_TARGET = w64-mingw32

# function that declares a compiler name for an architecture
# name.
define declare_compiler
	gcc_$(1) = $(1)-$(GCC_TARGET)-gcc
endef

#TODO: switch back to gnu-linux gcc and use fancy linking to make a portable
#executable anyway.

define add_source_dir
	source_dirs += ./source/$(1)
endef

source_dirs = ./source
output_dirs = ./build



arch_names = aarch64 x86_64 riscv64
arch_defs = __A64__ __X64__ __R64__
arch_snames = AA64   x64    RISCV64

$(eval $(call add_source_dir,kernel))
$(eval $(call add_source_dir,kernel/entry))
$(eval $(call add_source_dir,kernel/loaders))
$(eval $(call add_source_dir,kernel/loaders/common))
$(eval $(call add_source_dir,kernel/loaders/efi))
$(eval $(call add_source_dir,kernel/loaders/efi/internal))
$(eval $(call add_source_dir,kernel/machine))

$(foreach arch, $(arch_names), $(eval $(call declare_compiler,$(arch))))

asm_ext := S
ppc_ext := i
src_ext := c
obj_ext := o

src_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(src_ext)))
asm_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(asm_ext)))
ppc_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(ppc_ext)))
obj_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(obj_ext)))

efi_results = $(output_dirs)/bin/efi_root

include_dir := ./source

CFLAGS = --std=gnu17 -Werror -Wall -Wextra -Wpedantic -ffreestanding -mno-red-zone -I $(include_dir)
LFLAGS = -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main

define arch_template

asm_$(1) := $(subst source,build/asm,$(src_files:%.$(src_ext)=%$(1).$(asm_ext))) 
ppc_$(1) := $(subst source,build/ppc,$(src_files:%.$(src_ext)=%$(1).$(ppc_ext)))
obj_$(1) := $(subst source,build/obj,$(src_files:%.$(src_ext)=%$(1).$(obj_ext)))

$$(asm_$(1)) : $$(src_files) $$(asm_files)
	$(MKDIR) -p $$(dir $$@)
	$$(gcc_$(1)) $(CFLAGS) -D $(2) -S $$(subst build/asm,source,$$(patsubst %$(1).$(asm_ext),%.$(src_ext),$$@))  -o $$@

$$(ppc_$(1)) : $$(src_files) $$(ppc_files)
	$(MKDIR) -p $$(dir $$@)
	$$(gcc_$(1)) $(CFLAGS) -D $(2) -E $$(subst build/ppc,source,$$(patsubst %$(1).$(ppc_ext),%.$(src_ext),$$@))  -o $$@

$$(obj_$(1)) : $$(src_files) $$(asm_files) $$(ppc_files)
	$(MKDIR) -p $$(dir $$@)
	$(gcc_$(1)) $(CFLAGS) -D $(2) -c $$(subst build/obj,source,$$(patsubst %$(1).$(obj_ext),%.$(src_ext),$$@))  -o $$@

build_efi_$(1): $$(obj_$(1))
	$(MKDIR) -p ./build/bin/efi_root/boot
	$(gcc_$(1)) $(CFLAGS) $(LFLAGS) -D $(2) $$(obj_$(1)) -o ./build/bin/efi_root/boot/boot$(firstword $(3)).efi

endef


# the numbers for a word in the arch_names and arch_defines variables
# to disable an architecture, simply remove its number.
numbers = 2
$(foreach n, $(numbers), $(eval $(call arch_template,$(word $(n),$(arch_names)),$(word $(n),$(arch_defs)), $(word $(n),$(arch_snames)))))

make_disk:
	rm -rf ./build/iso
	$(MKDIR) -p ./build/iso
	dd if=/dev/zero of=build/iso/fat.img bs=1k count=1440
	mformat -i build/iso/fat.img -f 1440 ::
	mmd -i build/iso/fat.img ::/EFI
	mmd -i build/iso/fat.img ::/EFI/BOOT
	mcopy -i build/iso/fat.img build/bin/efi_root/boot/bootx64.efi ::/EFI/BOOT
	mkdir iso
	cp build/iso/fat.img iso
	xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o gos-efi.iso iso
	rm -rf iso


assemble: $(foreach an, $(arch_names), $(asm_$(an)))
preprocess: $(foreach an, $(arch_names), $(ppc_$(an)))
compile: $(foreach an, $(arch_names), $(obj_$(an)))
efi_link: $(foreach n, $(numbers), build_efi_$(word $(n), $(arch_names)))
build: assemble preprocess compile efi_link
	@echo done.

all:
	@echo $(source_dirs)
	@echo $(src_files)
	@echo $(asm_x86_64)

install:
	sudo apt install $(subst _,-,$(foreach an,$(arch_names), gcc-$(gmv)-$(an)-linux-gnu))