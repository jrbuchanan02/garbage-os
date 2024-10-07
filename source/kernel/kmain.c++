
#include <kernel/asm.h++>

extern "C" [[noreturn]] void kmain(void) {
    halt();
}