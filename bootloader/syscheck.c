#include <stdint.h>
#include <stdio.h>

void syscheck(void)
{
    // pentru prototip, afișăm doar că suntem aici
    volatile uint16_t* VGA = (uint16_t*)0xB8000;
    const char* msg = "Minux syscheck OK";
    for(int i=0; msg[i]; i++)
        VGA[i] = (0x0F << 8) | msg[i];
}
