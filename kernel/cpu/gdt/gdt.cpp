#include "gdt.h"
#include "../../../drivers/vga/vga.h"

extern "C" void gdt_flush(gdt::gdt_ptr_struct *);

gdt::gdt_entry gdt_entries[5];
gdt::gdt_ptr_struct gdt_ptr;

void gdt::init()
{
    gdt_ptr.limit = (sizeof(gdt_entry) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    add_gdt_descriptor(0, 0, 0, 0, 0);                // NULL Segment
    add_gdt_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code Segment
    add_gdt_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data Segment
    add_gdt_descriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code Segment
    add_gdt_descriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data Segment

    gdt_flush(&gdt_ptr);

    vga::kprintf("Finished Initializing GDT\n");
}

void gdt::add_gdt_descriptor(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt_entries[index].base_low = (base_address & 0xFFFF);
    gdt_entries[index].base_middle = (base_address >> 16) & 0xFF;
    gdt_entries[index].base_high = (base_address >> 24) & 0xFF;

    gdt_entries[index].limit = (limit & 0xFFFF);

    gdt_entries[index].flags = (limit >> 16) & 0x0F;
    gdt_entries[index].flags |= (flags & 0xF0);

    gdt_entries[index].asccess = access;
}