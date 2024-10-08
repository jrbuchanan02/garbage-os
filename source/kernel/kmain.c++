
#include <kernel/asm.h++>

extern "C" [[noreturn]] void kmain(void) {
    while(true) halt();
}