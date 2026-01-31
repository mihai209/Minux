#include <stdint.h>

// Multiboot2 header
__attribute__((section(".multiboot")))
const uint32_t multiboot_header[] = {
    0xE85250D6,                                 // magic
    0,                                          // architecture
    (3 * 4) + 8,                                // header length (24 bytes)
    -(0xE85250D6 + 0 + (3 * 4) + 8 + 0x00000000), // checksum
    0,                                          // end tag (type)
    0,                                          // end tag (flags)
    8,                                          // end tag (size)
};

// VGA text buffer
#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 0x0F // white on black

static volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDR;
static uint16_t vga_row = 0;
static uint16_t vga_col = 0;

// Clear the screen
static void vga_clear(void)
{
    uint16_t blank = (' ' | (VGA_COLOR << 8));
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buffer[i] = blank;
    vga_row = 0;
    vga_col = 0;
}

// Move cursor to next line
static void vga_newline(void)
{
    vga_row++;
    vga_col = 0;
    
    if (vga_row >= VGA_HEIGHT) {
        // Scroll up
        for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        
        // Clear bottom line
        uint16_t blank = (' ' | (VGA_COLOR << 8));
        for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++)
            vga_buffer[i] = blank;
        
        vga_row = VGA_HEIGHT - 1;
    }
}

// Print a single character
static void vga_putchar(char c)
{
    if (c == '\n') {
        vga_newline();
        return;
    }
    
    if (c == '\r') {
        vga_col = 0;
        return;
    }
    
    uint16_t index = vga_row * VGA_WIDTH + vga_col;
    vga_buffer[index] = (c | (VGA_COLOR << 8));
    
    vga_col++;
    if (vga_col >= VGA_WIDTH)
        vga_newline();
}

// Print a string to VGA text buffer
void kprint(const char *msg)
{
    if (!msg)
        return;
    
    for (int i = 0; msg[i]; i++)
        vga_putchar(msg[i]);
}

// Main kernel entry point
void kmain(void)
{
    // Initialize VGA
    vga_clear();
    
    // Print startup message
    kprint("Minux kernel online\n");
    kprint("Welcome to Minux OS!\n");
    kprint("Running in x86_64 long mode\n");
    
    // Halt CPU
    for (;;)
        __asm__ volatile("hlt");
}
