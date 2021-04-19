GLOBAL_C_FLAGS := -c -Werror -Wall -Wextra -Wpedantic -ffreestanding -mno-red-zone -mtune=generic

loader_source = $(shell find code/kernel/boot/ -name *.S)
loader_object = $(patsubst code/kernel/boot/%.S, comp/kernel/boot/%.o, $(loader_source))

initializer_source = $(shell find code/kernel/boot/ -name *.c)
initializer_object = $(patsubst code/kernel/boot/%.c, comp/kernel/boot/%.o, $(initializer_source))

all: preclean link clean

preclean:
	rm -r -f disk/result/*.bin disk/result/*.iso disk/iso/boot/*.bin

link: $(loader_object) $(initializer_object)

	ld -A i386 -b elf32-i386 -m elf_i386 -T link/link.ld -o disk/iso/boot/bootloading.bin $(loader_object) $(initializer_object)

	grub-mkrescue /usr/lib/grub/i386-pc -o disk/result/gosx64.iso disk/iso

clean:
	rm -r -f comp/*.o
$(loader_object) : $(loader_source)
	gcc $(GLOBAL_C_FLAGS) -O0 -D__LONG_MODE__=1 -m32 -o $@ $(patsubst comp/kernel/boot/%.o, code/kernel/boot/%.S, $@)

$(initializer_object) : $(initializer_source)
	gcc $(GLOBAL_C_FLAGS) -m32 -I header/kernel/boot/ -o $@ $(patsubst comp/kernel/boot/%.o, code/kernel/boot/%.c, $@)