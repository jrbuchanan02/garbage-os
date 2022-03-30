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

define add_source_dir
	source_dirs += ./source/$(1)
endef

source_dirs = ./source

arch_names = aarch64 x86_64 riscv64
arch_defs = __A64__ __X64__ __R64__

$(eval $(call add_source_dir,kernel))
$(eval $(call add_source_dir,kernel/entry))
$(eval $(call add_source_dir,kernel/loaders))
$(eval $(call add_source_dir,kernel/loaders/efi))
$(eval $(call add_source_dir,kernel/loaders/efi/internal))
$(eval $(call add_source_dir,kernel/machine))

asm_ext := S
ppc_ext := i
src_ext := c
obj_ext := o

src_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(src_ext)))
asm_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(asm_ext)))
ppc_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(ppc_ext)))
obj_files = $(foreach dir, $(source_dirs), $(wildcard $(dir)/*.$(obj_ext)))

include_dir := ./source

CFLAGS = --std=gnu17 -Werror -Wall -Wextra -Wpedantic -ffreestanding -I $(include_dir)

define arch_template

asm_$(1) := $(subst source,build/asm,$(src_files:%.$(src_ext)=%$(1).$(asm_ext))) 
ppc_$(1) := $(subst source,build/ppc,$(src_files:%.$(src_ext)=%$(1).$(ppc_ext)))
obj_$(1) := $(subst source,build/obj,$(src_files:%.$(src_ext)=%$(1).$(obj_ext)))

$$(asm_$(1)) : $$(src_files) $$(asm_files)
	mkdir -p $$(dir $$@)
	$(1)-linux-gnu-gcc-10 $(CFLAGS) -D $(2) -S $$(subst build/asm,source,$$(patsubst %$(1).$(asm_ext),%.$(src_ext),$$@))  -o $$@

$$(ppc_$(1)) : $$(src_files) $$(ppc_files)
	mkdir -p $$(dir $$@)
	$(1)-linux-gnu-gcc-10 $(CFLAGS) -D $(2) -E $$(subst build/ppc,source,$$(patsubst %$(1).$(ppc_ext),%.$(src_ext),$$@))  -o $$@

$$(obj_$(1)) : $$(src_files) $$(asm_files) $$(ppc_files)
	mkdir -p $$(dir $$@)
	$(1)-linux-gnu-gcc-10 $(CFLAGS) -D $(2) -c $$(subst build/obj,source,$$(patsubst %$(1).$(obj_ext),%.$(src_ext),$$@))  -o $$@
endef
# the numbers for a word in the arch_names and arch_defines variables
numbers = 1 2 3
$(foreach n, $(numbers), $(eval $(call arch_template,$(word $(n),$(arch_names)),$(word $(n),$(arch_defs)))))

assemble: $(foreach an, $(arch_names), $(asm_$(an)))
preprocess: $(foreach an, $(arch_names), $(ppc_$(an)))
compile: $(foreach an, $(arch_names), $(obj_$(an)))
build: assemble preprocess compile
	mkdir -p ./build/bin
	

all:
	@echo $(source_dirs)
	@echo $(src_files)
	@echo $(asm_x86_64)

install:
	sudo apt install $(subst _,-,$(foreach an,$(arch_names), gcc-10-$(an)-linux-gnu))