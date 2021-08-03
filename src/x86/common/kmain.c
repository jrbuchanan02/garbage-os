#include"multiboot2.h"
#define ___ASMCALLINTF___ __attribute__((fastcall))


#define ___NOTMULTIBOOT2PANIC 1
#define ___UNALIGNEDMBI       2
/**
 * @brief Sets the GDT to the specified gdt pointer and size
 * @note The algorithm assumes the kernel code offset to be 0x8 and
 * the kernel data offset to be 0x10. Make your gdt's accordingly.
 * @param gdt the GDT
 * @param size the size of the table (must be <= 65535)
 */
extern void setgdt(unsigned long gdt, unsigned long size);

void basicpanic(register unsigned char reason){
    // write, assuming that CGA / EGA exist, E: [reason in the CGA / EGA character set]
    // onto the screen.
    static unsigned char *egabuffer=(unsigned char *)0xb8000;
    egabuffer[0]='E';
    egabuffer[1]=0x4f;
    egabuffer[2]=':';
    egabuffer[3]=0x4f;
    egabuffer[4]=' ';
    egabuffer[5]=0x4f;
    egabuffer[6]=reason;
    egabuffer[7]=0x4f;
    __asm__("cli"); // clear all interrupts
    __asm__("hlt"); // halt the system
}

void ___ASMCALLINTF___ kmain(unsigned long magic, unsigned long mb2){
    //static unsigned long size;
    static struct multiboot_tag *tag;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) basicpanic(___NOTMULTIBOOT2PANIC);
    if (mb2 & 7) basicpanic(___UNALIGNEDMBI);
    //size=*(unsigned *)mb2; // size is the first entry in the mbi
    tag=(struct multiboot_tag *)(mb2+8); // set the tag
    tag=tag; // we'll use tag later, but first, we should set up things like the
             // gdt, idt, and some ldts
    __asm__("hlt");
}