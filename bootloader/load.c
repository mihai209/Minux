#include "load.h"
#include <stdint.h>

extern void kmain(void);

void load_main(void)
{
    // Pentru început, doar jump direct în kernel
    // mai târziu vom încărca ELF-ul
    kmain();
    for(;;) __asm__ volatile("hlt");
}
