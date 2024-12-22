#pragma once

#include <stdint.h>

namespace gdt
{
    typedef struct gdt_entry
    {
        uint16_t limit;
        uint16_t base_low;
        uint8_t base_middle;
        uint8_t asccess;
        uint8_t flags;
        uint8_t base_high;
    } __attribute__((packed)) gdt_entry;

    typedef struct gdt_ptr_struct
    {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) gdt_ptr_struct;

    void init();
    void add_gdt_descriptor(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t flags);
}