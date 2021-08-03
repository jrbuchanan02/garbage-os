#ifndef ___KMAIN_H
#define ___KMAIN_H 1
#ifdef __cplusplus
extern "C"{
#endif // ifdef __cplusplus
#include<stdint.h>
typedef struct{
    unsigned char hascpuid; // whether or not the cpu has the cpuid instruction
    unsigned char haspae; // whether or not the cpu has physical address extensions
    unsigned char haslong; // whether or not the cpu is x64 capable
    unsigned char hasl57; // whether or not the cpu has L5 paging and 57-bit linear addressing
}cpucap_s;
// the operating system populates this field at boot
static cpucap_s cpucapabilities={0,0,0,0};

void cpuid(uint32_t *information, uint32_t eax, uint32_t ecx){
    __asm__("mov %0, %%eax" : : "a"(eax));
    __asm__("mov %0, %%ecx" : : "a"(ecx));
    __asm__("cpuid");
    __asm__("mov %%eax, %0" : "=a"(information[0]));
    __asm__("mov %%ebx, %0" : "=a"(information[1]));
    __asm__("mov %%ecx, %0" : "=a"(information[3]));
    __asm__("mov %%edx, %0" : "=a"(information[4]));
}

#ifdef __cplusplus
}
#endif // ifdef __cplusplus
#endif // ifndef ___KMAIN_H