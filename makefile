kernel_source_files := $(shell find gos/intel64/code/kernel/ -name *.d)
kernel_object_files := $(patsubst gos/intel64/code/kernel/%.d, gos/build/intel64/kernel/%.o, $(kernel_source_files))



intel64_asm_source_files := $(shell find gos/intel64/code/ -name *.s)
intel64_asm_object_files := $(patsubst gos/intel64/code/%.s, gos/build/intel64/%.o, $(intel64_asm_source_files))

$(kernel_object_files): gos/build/intel64/kernel/%.o :gos/intel64/code/kernel/%.d
	mkdir -p $(dir $@) && \
	ldc2 -c --betterC $(patsubst gos/build/intel64/kernel/%.o, gos/intel64/code/kernel/%.d, $@) -od=gos/build/intel64/kernel

$(intel64_asm_object_files): gos/build/intel64/%.o : gos/intel64/code/%.s
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst gos/build/intel64/%.o, gos/intel64/code/%.s, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(intel64_asm_object_files)
	mkdir -p gos/dist/intel64 && \
	ld -b elf64-x86-64 -n -o gos/dist/intel64/kernel.bin -T gos/targets/intel64/linker.ld $(intel64_asm_object_files) $(kernel_object_files) && \
	cp gos/dist/intel64/kernel.bin gos/targets/intel64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o gos/dist/intel64/kernel.iso gos/targets/intel64/iso