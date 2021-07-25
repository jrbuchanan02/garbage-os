GLOBAL_C_FLAGS := -c -Werror -Wall -Wextra -Wpedantic -ffreestanding -mno-red-zone -mtune=generic


asm_x64_src := $(wildcard ./src/x86/common/*.S) $(wildcard ./src/x86/x64/*.S)

asm_x64_obj := $(patsubst ./src/x86/common/%.S, ./obj/x86/common/x64/%.o, $(patsubst ./src/x86/x64/%.S, ./obj/x86/x64/%.o, $(asm_x64_src))) 


all:
	@echo $(asm_x64_src)
	@echo $(asm_x64_obj)