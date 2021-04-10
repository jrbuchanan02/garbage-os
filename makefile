kernel_sources := $(shell find gos/code/impl/kernel/ -name *.c)
kernel_objects := $(patsubst gos/code/impl/kernel/%.c, gos/build/impl/kernel/%.o, $(kernel_sources))


all_objects := $(kernel_objects) 
mkbootimg := $(shell find mkbootimg/mkbootimg)

define losetup_limits
8388608
endef
# define losetup_offset as the 2048th sector
define losetup_offset
1048576
endef

$(kernel_objects): $(kernel_sources)
	gcc -ffreestanding -mno-red-zone -O3 -c -o $@ $(patsubst gos/build/impl/kernel/%.o, gos/code/impl/kernel/%.c, $@)
all: preclean build_kernel document clean

preclean:
	rm -rf gos/dist/output.bin
	rm -rf gos/disk/*.*
build_kernel: $(all_objects)
	ld -nostdlib -nostartfiles -T gos/code/link/link.ld -o gos/dist/BOOT/KERN/output.bin -b elf64-x86-64 $(all_objects)
	$(mkbootimg) check gos/dist/BOOT/KERN/output.bin

	dd if=/dev/zero of=gos/disk/gos.img bs=1048576 count=512
	(echo "g"; echo "n p"; echo "1"; echo "2048"; echo "+8M"; echo "t 1"; echo "1"; echo "w") | fdisk gos/disk/gos.img
	sudo losetup -o $(losetup_offset) --sizelimit $(losetup_limits) -f gos/disk/gos.img
	sudo mkfs.vfat -F 16 -n "EFI System" /dev/loop0
	sudo mount /dev/loop0 gos/dist
	#any file copying goes here
	sudo umount gos/dist
	./mkbootimg/mkbootimg gos/code/bootboot/configuration.json gos/disk/gos.img
document:
	doxygen gos/code/doc/doxyfile.cfg gos/code/doc/
clean:
	
	rm -rf gos/build/impl/kernel/*.o
	rm -rf gos/build/impl/font/*.o
	sudo losetup -d /dev/loop0