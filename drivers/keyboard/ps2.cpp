#include "ps2.h"

#include "../vga/vga.h"
#include "../ports/ports.h"
#include "../vga/colors.h"

void ps2::init()
{
    outb(0x64, 0xAD); // Disable PS/2 port 1
    outb(0x64, 0xA7); // Disable PS/2 port 2

    // Flush the output buffer
    inb(0x60);

    // Enable PS/2 port 1
    outb(0x64, 0xAE);

    // Enable keyboard interrupts
    outb(0x64, 0x20); // Read command byte
    uint8_t cmd = inb(0x60);
    cmd |= 0x01;      // Enable interrupt on PS/2 port 1
    outb(0x64, 0x60); // Write command byte
    outb(0x60, cmd);

    idt::irqInstallHandler(1, &ps2::handler);

    vga::kprintf("Finished Initializing PS2 Keyboard\n");
}

void ps2::handler(idt::stack_frame *frame)
{
    char scanCode = inb(0x60); // key code
    if (scanCode == -0x56)     // shift relese
    {
        shift_pressed = false;
    }

    if (scanCode < 0 || scanCode == 0x4B || scanCode == 0x4D) // relesed or arrows
    {
        return;
    }

    if (scanCode == 0x48) // scroll up
    {
        vga::update_screen(false);
    }

    else if (scanCode == 0x50) // scroll down
    {
        vga::update_screen(true);
    }

    else if (scanCode == 0x3A) // caps lock
    {
        caps_pressed = !caps_pressed;
    }

    else if (scanCode == 0x2A) // shift press
    {
        shift_pressed = true;
    }

    else if (shift_pressed && caps_pressed)
    {
        vga::put_char(sc_ascii_caps_shift[scanCode], BG_BLACK | TEXT_WHITE);
    }

    else if (shift_pressed)
    {
        vga::put_char(sc_ascii_shift[scanCode], BG_BLACK | TEXT_WHITE);
    }

    else if (caps_pressed)
    {
        vga::put_char(sc_ascii_caps[scanCode], BG_BLACK | TEXT_WHITE);
    }

    else
    {
        vga::put_char(sc_ascii[scanCode], BG_BLACK | TEXT_WHITE);
    }
}