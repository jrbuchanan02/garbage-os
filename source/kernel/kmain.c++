
#include <kernel/asm.h++>
#include <kernel/types.h++>

extern "C" [[noreturn]] void kmain(std::size_t thread_id) {
    if (thread_id == 0) {
        while(true) halt();
    } else {
        while(true);
    }
}