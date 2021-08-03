GLOBAL_C_FLAGS := -c -Werror -Wall -Wextra -Wpedantic -ffreestanding -mno-red-zone -mtune=generic
GLOBAL_S_FLAGS := 

MB2X86COMDIR := ./src/x86/common

MB2X86COMNASMDEF := -D___X86___
MB2X86COMGCCARGS := -D___X86___ -m32 -march=i386

MB2X86COMLINKERF := ./linkx86com.ld

x86com_asm := $(wildcard $(MB2X86COMDIR)/*.S)
x86com_src := $(wildcard $(MB2X86COMDIR)/*.c) # srC for source C

x86com_asmobj := $(patsubst ./src/%.S, ./obj/%.o, $(x86com_asm))
x86com_srcobj := $(patsubst ./src/%.c, ./obj/%.o, $(x86com_src))

$(x86com_asmobj) : $(x86com_asm)
#permissively ensure that we have our directory
	mkdir -p $(dir $@)
	nasm -f elf32 $(MB2X86COMNASMDEF) $(patsubst obj/%.o, src/%.S, $@) -o $@
$(x86com_srcobj) : $(x86com_src)
	mkdir -p $(dir $@)
	gcc $(GLOBAL_C_FLAGS) $(MB2X86COMGCCARGS) $(patsubst obj/%.o, src/%.c, $@) -o $@

mb2linkx86com: $(x86com_asmobj) $(x86com_srcobj)
	ld -A i386 -m elf_i386 $(x86com_asmobj) $(x86com_srcobj) -T $(MB2X86COMLINKERF) -obin/x86/common/mb2.bin

mb2makex8664: mb2linkx86com
	cp ./bin/x86/common/mb2.bin ./dst/x86/x64/iso/boot/mb2.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o dst/gosmb2x64.iso dst/x86/x64/iso

all: mb2makex8664


clean:
# remove all object files from the toolchain
# TODO: fix
	rm -rf ./obj/*.o