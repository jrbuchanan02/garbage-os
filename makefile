# find all the source files within the kernel
kernel_sources := $(shell find gos/code/impl/kernel/ -name *.c)
# change the source file names to what they will be after compilation
kernel_objects := $(patsubst gos/code/impl/kernel/%.c, gos/build/impl/kernel/%.o, $(kernel_sources))

# find all the source files within the CRT
crt_sources := $(shell find gos/code/impl/crt/ -name *.c)
# change the source file names to what they will be after compilation
crt_objects := $(patsubst gos/code/impl/crt/%.c, gos/build/impl/crt/%.o, $(crt_sources))

# find all the OS source files
os_sources := $(shell find gos/code/impl/os/ -name *.c)
# change their names to what they will be after compilation
os_objects := $(patsubst gos/code/impl/os/%.c, gos/build/impl/os/%.o, $(os_sources))

gos_binary := $(patsubst gos/code/impl/os/%.c, gos/dist/usr/os/%.elf, $(os_sources))
C_FLAGS := -ffreestanding -mno-red-zone -Wall -Wextra -Wpedantic -Werror -O0 -g -c
LDFLAGS := -nostdlib -nostartfiles -b elf64-x86-64

all_objects := $(kernel_objects) $(crt_objects) $(os_objects)
# find the mkbootimg file
mkbootimg := $(shell find mkbootimg/mkbootimg)

# 8 * 1024 * 1024
losetup_limits = 8388608
# define losetup_offset as the 2048th sector
# 512 * 2048
losetup_offset = 1048576

# for make all -B, do this	
all: preclean build_kernel document clean

## x86-64 (aka I32e Intel64 amd64)
#x64: precleanx64 build_kernelx64 cleanx64
## AARCH64 (aka ARMv8)
#a64: precleana64 build_kernela64 cleana64
## RISC-V 64
#v64: precleanv64 build_kernelv64 cleanv64
# also include this handy function, note that this makefile assumes its loop device is 
# /dev/loop0. Any already open loop devices will make this assumption invalid
looplist:
	@echo "If you see nothing after \"losetup -a\", then no loop devices are currently in use and you're good to go!"
	losetup -a

# build the kernel's objects.
# we will consider the kernel separate from the rest of the Operating System and from the C runtime
# the kernel will be only capable of initializing these things:
#
# 1. GDT, IDT, and LDT
# 2. itself (duh)
# 3. the filesystem. 
#
# after carrying out these critical tasks, the kernel will load the other modules into RAM and ensure that
# they are contantly running (even if in the background).
#
# Some modules separate from the kernel will include the Garbage Collector, Task Switcher, and other stuff that may 
# run in the privlege level 0. This design is definitely that of a microkernel.
$(kernel_objects) : $(kernel_sources)
	gcc $(C_FLAGS) -c -o $@ $(patsubst gos/build/impl/kernel/%.o, gos/code/impl/kernel/%.c, $@)
# The C Runtime is compiled separately from the kernel. The kernel will run entirely without API or Runtime.
# The C Runtime will eventually conform to the (latest) ISO C Standard. However, that's a big *in the future* item
$(crt_objects) : $(crt_sources)
	gcc $(C_FLAGS) -c -o $@ $(patsubst gos/build/impl/crt/%.o, gos/code/impl/crt/%.c, $@)

# Here we will compile separate modules within the operating system. 
# the goal is to compile everything in the operating system such that it pedantically complies with the standard, does not warn about anything,
# does not crash or act unstable, and is light enough for application developers to take advantage of the hardware.
#
# That paragraph being written, note that I will not take steps to aggressively optimize the Operating System code - the goal of Garbage OS is to
# have a built-in Garbage Collector, not to be the fastest Operating System in existence.
$(os_objects) : $(os_sources)
	gcc $(C_FLAGS) -c -o $@ $(patsubst gos/build/impl/os/%.o, gos/code/impl/os/%.c, $@)
$(gos_binary) : $(os_objects)
	ld $(LDFLAGS) -T gos/code/link/os.ld -o $@ $(patsubst gos/dist/usr/os/%.elf, gos/build/impl/os/%.o, $@)
# Ensure that the disk is empty before building - removed files and name changes may occur.
preclean:
	rm -rf gos/dist/*.bin
	rm -rf gos/dist/usr/os/*.elf
	rm -rf gos/disk/*.*
#build everything
build_kernel: $(all_objects) $(gos_binary)
	ld $(LDFLAGS) -T gos/code/link/kernel.ld -o gos/dist/boot/kern/output.bin $(kernel_objects)
	ld $(LDFLAGS) -T gos/code/link/crt.ld -o gos/dist/usr/crt/crt.bin $(crt_objects)
	$(mkbootimg) check gos/dist/boot/kern/output.bin
# create the disk image
	dd if=/dev/zero of=gos/disk/gos.img bs=1048576 count=512
	(echo "g"; echo "n p"; echo "1"; echo "2048"; echo "+8M"; echo "t 1"; echo "1"; echo "w") | fdisk gos/disk/gos.img
	sudo losetup -o $(losetup_offset) --sizelimit $(losetup_limits) -f gos/disk/gos.img
	sudo mkfs.vfat -F 16 -n "EFI System" /dev/loop0
	sudo mount /dev/loop0 gos/dist
# any file copying goes here
	sudo umount gos/dist
	./mkbootimg/mkbootimg gos/code/bootboot/configuration.json gos/disk/gos.img
document:
	rm -rf docs/*.html
	rm -rf docs/*.js
	rm -rf docs/*.css
	doxygen Doxyfile.cfg
clean:
	rm -rf gos/build/impl/crt/*.o
	rm -rf gos/build/impl/kernel/*.o
	rm -rf gos/build/impl/font/*.o
	sudo losetup -d /dev/loop0