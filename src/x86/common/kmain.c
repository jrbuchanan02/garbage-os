#include"multiboot2.h"
#include"kmain.h"
#define ___ASMCALLINTF___ __attribute__((fastcall))

// panic due to incorrect multiboot2 magic number
#define ___NOTMULTIBOOT2PANIC 1
// panic due to incorrectly aligned mbi structure
#define ___UNALIGNEDMBI       2
/**
 * @brief Sets the GDT to the specified gdt pointer and size
 * @note The algorithm assumes the kernel code offset to be 0x8 and
 * the kernel data offset to be 0x10. Make your gdt's accordingly.
 * @param gdt the GDT
 * @param size the size of the table (must be <= 65535)
 */
extern void setgdt(unsigned long gdt, unsigned long size);
/**
 * @brief Tells us whether or not the cpuid instruction is available on the
 * system
 * @return 
 */
extern unsigned char hascpuid();
/**
 * Basic panic routine that indicates the error code.
 * @param reason the error code (one of 256) that tells the end user
 * (or the stupid developer) what went wrong this early in the boot
 * process.
 */
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
/**
 * @brief Cute function that, like the text in _start32, prints out the architecture
 * on the screen with a green background. Except, instead of using a question mark, 
 * `processorprint` uses an exclamation point.
 */
void processorprint(){
    static unsigned char *egabuffer=(unsigned char *)0xb8000;
    if (cpucapabilities.hasl57){
        egabuffer[0]='L';
        egabuffer[1]=0x2f;
        egabuffer[2]='5';
        egabuffer[3]=0x2f;
        egabuffer[4]='7';
        egabuffer[5]=0x2f;
        egabuffer[6]='!';
        egabuffer[7]=0x2f;
    } else if (cpucapabilities.haslong){
        egabuffer[0]='X';
        egabuffer[1]=0x2f;
        egabuffer[2]='6';
        egabuffer[3]=0x2f;
        egabuffer[4]='4';
        egabuffer[5]=0x2f;
        egabuffer[6]='!';
        egabuffer[7]=0x2f;
    } else if (cpucapabilities.haspae){
        egabuffer[0]='P';
        egabuffer[1]=0x2f;
        egabuffer[2]='A';
        egabuffer[3]=0x2f;
        egabuffer[4]='E';
        egabuffer[5]=0x2f;
        egabuffer[6]='!';
        egabuffer[7]=0x2f;
    } else{
        egabuffer[0]='X';
        egabuffer[1]=0x2f;
        egabuffer[2]='8';
        egabuffer[3]=0x2f;
        egabuffer[4]='6';
        egabuffer[5]=0x2f;
        egabuffer[6]='!';
        egabuffer[7]=0x2f;
    }
}

/**
 * @brief Entry point for the operating system
 * @note Defined as "fastcall" because fastcall directly uses ecx and edx as arguments,
 * which should be easier for my "bad" assembly programming.
 * @param magic the multiboot2 magic number
 * @param mb2 the pointer to the multiboot2 information structure
 */
void ___ASMCALLINTF___ kmain(unsigned long magic, unsigned long mb2){
    //static unsigned long size;
    static struct multiboot_tag *tag;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) basicpanic(___NOTMULTIBOOT2PANIC);
    if (mb2 & 7) basicpanic(___UNALIGNEDMBI);
    //size=*(unsigned *)mb2; // size is the first entry in the mbi
    tag=(struct multiboot_tag *)(mb2+8); // set the tag
    tag=tag;
    // determine the capabilities of our system.
    cpucapabilities.hascpuid=hascpuid();
    uint32_t cpuinfo[4]={0,0,0,0};
    if(cpucapabilities.hascpuid){
        cpuid(cpuinfo,0,0);
        if(cpuinfo[0]<1) cpucapabilities.haspae=0;
        else{
            cpuid(cpuinfo,1,0);
            if(cpuinfo[3]&1<<6)cpucapabilities.haspae=1;
            else cpucapabilities.haspae=0;
        }
        if(cpucapabilities.haspae){
            // check for long mode
            cpuid(cpuinfo, 0x80000000, 0);
            if(cpuinfo[0]&0x7fffFFFF){ // did we get any value other than what we just put in?
                cpuid(cpuinfo,0x80000001,0);
                if(cpuinfo[3]&1<<29)cpucapabilities.haslong=1;
                else cpucapabilities.haslong=0;
            }
        }
        if(cpucapabilities.haslong){
            cpuid(cpuinfo,7,0);
            if(cpuinfo[2]&1<<16)cpucapabilities.hasl57=1;
            else cpucapabilities.hasl57=0;
        }
    }
    processorprint();
    __asm__("hlt");
}