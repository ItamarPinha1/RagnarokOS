#pragma once

#include <stdint.h>

namespace pic
{
    void init();
    void sendEOI(uint8_t irq);
    void remap(int offset1, int offset2);
}