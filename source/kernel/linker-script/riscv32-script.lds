/* Adapted from https://wiki.osdev.org/RISC-V_Bare_Bones 
 * Adapted, rather than written from scratch since I don't actually
 * know the qemu-system-riscv32 virtual machine's memory map
 */

ENTRY(start);
. = 0x80000000;

SECTIONS {
    .text : ALIGN(4K) {
        *(.init);
        *(.text);
    }
    .bss : ALIGN(4K) {
        PROVIDE(bss_start = .);
        *(.bss);
        . += 4096;
        PROVIDE(stack_top = .);
        . += 4096;
        PROVIDE(global_pointer = .);
        PROVIDE(bss_end = .);
    }
    .rodata : ALIGN(4K) {
        *(.rodata);
    }
    .data : ALIGN(4K) {
        *(.data);
    }
}