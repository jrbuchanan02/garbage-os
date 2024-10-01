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

# require C++ 20, freestanding, no exceptions
# emit all warnings
# don't link
# make source one of the include directories
# turn on basic optimizations
loader_flags = --std=c++20 -ffreestanding -fno-exceptions -Wall -Wextra -Wpedantic -c -I source -O1

define loader_arch_source_template = 
	@echo Building $@
	@mkdir -p $(@D)
	@$(1)-linux-gnu-g++ $(loader_flags) -o $@ $<
endef

define loader_arch_elf_file_template = 
	@echo Linking $@
	@mkdir -p $(@D)
	@$(1)-linux-gnu-ld -e efi_main -o $@ $^
endef

loader_sources = source/loader/efimain.c++ source/loader/efi/status.c++

loader_x86_64_boot_efi_elf_objects = $(loader_sources:source/%.c++=build/x86_64/%.o)
loader_aarch64_boot_efi_elf_objects = $(loader_sources:source/%.c++=build/aarch64/%.o)

build/x86_64/loader/%.o: source/loader/%.c++
	$(call loader_arch_source_template,x86_64)
build/aarch64/loader/%.o: source/loader/%.c++
	$(call loader_arch_source_template,aarch64)

# build/x86_64/loader/efimain.o: source/loader/efimain.c++
# 	$(loader_x86_64_source)

# build/x86_64/loader/efi/status.o: source/loader/efi/status.c++
# 	$(loader_x86_64_source)

bin/x86_64/loader/BOOT.elf: $(loader_x86_64_boot_efi_elf_objects)
	$(call loader_arch_elf_file_template,x86_64)

bin/aarch64/loader/BOOT.elf: $(loader_aarch64_boot_efi_elf_objects)
	$(call loader_arch_elf_file_template,aarch64)

result/loader/x86_64/BOOT.EFI: bin/x86_64/loader/BOOT.elf
	@echo Converting $< into $@
	@mkdir -p $(@D)
	@x86_64-linux-gnu-objcopy -O pei-x86-64 --subsystem 10 $< $@
result/loader/aarch64/BOOT.EFI: bin/aarch64/loader/BOOT.elf
	@echo Converting $< into $@
	@mkdir -p $(@D)
	@aarch64-linux-gnu-objcopy -O pei-aarch64-little --subsystem 10 $< $@

loader: result/loader/x86_64/BOOT.EFI result/loader/aarch64/BOOT.EFI

efi_disk: loader
# make the iso workspace
	mkdir -p result/x86_64/iso
	mkdir -p result/aarch64/iso
# make the boot folder in the ISO file
	mkdir -p result/x86_64/iso/BOOT
	mkdir -p result/aarch64/iso/BOOT
# move the loader files into the boot directories
	mv result/loader/x86_64/BOOT.EFI result/x86_64/iso/BOOT/BOOT.EFI
	mv result/loader/aarch64/BOOT.EFI result/aarch64/iso/BOOT/BOOT.EFI
# make the ISO images
	xorriso -x -outdev GOS-x86_64.iso -map result/x86_64/iso /
	xorriso -x -outdev GOS-aarch64.iso -map result/aarch64/iso / -boot_image "efi_boot_part=--efi-boot-image"
clean:
	rm -rf bin
	rm -rf build
	rm -rf result