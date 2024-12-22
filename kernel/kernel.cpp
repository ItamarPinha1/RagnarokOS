#include "../drivers/vga/vga.h"
#include "cpu/gdt/gdt.h"
#include "cpu/interrupts/idt.h"
#include "../drivers/keyboard/ps2.h"
#include "../boot/multiboot.h"
#include "memory/pmm.h"

extern "C" void kmain(multiboot::Info *mb)
{
    vga::init();
    gdt::init();
    idt::init();
    ps2::init();

    pmm::init(mb);

    while (1)
    {
    }
}
