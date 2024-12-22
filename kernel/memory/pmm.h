#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../../boot/multiboot.h"

namespace pmm
{
    void init(multiboot::Info *mbInfo);
    uint32_t allocPage();
    void freePage(uint32_t address);

    constexpr size_t PAGE_SIZE = 4096; // 4 KB
}
