C__FLAG := -c -fno-stack-protector -fpic -fshort-wchar -ffreestanding -mno-red-zone -maccumulate-outgoing-args -I/usr/include/efi -I/usr/include/efi/x86_64 -DEFI_FUNCTION_WRAPPER

C__CODE := $(wildcard ./src/*.c) $(wildcard ./src/boot/*.c)

C__OBJS := $(patsubst ./src/%.c, ./obj/%.o, $(C__CODE))

OS__EFI := bin/gos.elf
OS__BIN := ./bin/gos.efi
OS__ISO := bin/gos.iso

OS__TMP := /tmp/gos.iso

$(C__OBJS) : $(C__CODE)
	mkdir -p $(dir $@)
	gcc $(patsubst obj/%.o, src/%.c, $@) $(C__FLAG) -o $@


all : $(C__OBJS)
	mkdir -p $(dir $(OS__EFI))
	mkdir -p $(dir $(OS__BIN))
	ld -shared -Bsymbolic -L/usr/lib -T/usr/lib/elf_x86_64_efi.lds /usr/lib/crt0-efi-x86_64.o $(C__OBJS) -o $(OS__EFI) -lgnuefi -lefi
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 $(OS__EFI) $(OS__BIN)
	parted $(OS__ISO) -s -a minimal mklabel gpt
	parted $(OS__ISO) -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted $(OS__ISO) -s -a minimal toggle 1 boot
	dd if=/dev/zero of=$(OS__TMP) bs=512 count=91669
	mformat -i $(OS__TMP) -h 32 -t 32 -n 64 -c 1
	rm -f gos.efi
	mcopy -i $(OS__TMP) $(OS__BIN)
	cp -f gos.efi bin/gos.efi
	rm -f gos.efi
# ---- other applications go here ----
	dd if=$(OS__TMP) of=$(OS__ISO) bs=512 count=91669 seek=2048 conv=notrunc

run: all
	~/.cargo/bin/uefi-run -b /usr/share/qemu/OVMF.fd -q /usr/bin/qemu-system-x86_64 ./bin/gos.efi -- -cpu qemu64