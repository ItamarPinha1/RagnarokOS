#include "pmm.h"
#include "../../drivers/vga/vga.h"
#include "../../boot/multiboot.h"

// Global variables
static uint8_t *bitmap = nullptr;
static uint32_t totalPages = 0;
static uint32_t bitmapSize = 0; // Bitmap size in bytes

namespace pmm
{
    void markPagesReserved(uint32_t start, uint32_t length)
    {
        uint32_t pageStart = start / PAGE_SIZE;
        uint32_t pageCount = length / PAGE_SIZE;

        for (uint32_t i = 0; i < pageCount; i++)
        {
            uint32_t pageIndex = pageStart + i;
            bitmap[pageIndex / 8] |= (1 << (pageIndex % 8)); // Mark as reserved
        }
    }

    void markPagesFree(uint32_t start, uint32_t length)
    {
        uint32_t pageStart = start / PAGE_SIZE;
        uint32_t pageCount = length / PAGE_SIZE;

        for (uint32_t i = 0; i < pageCount; i++)
        {
            uint32_t pageIndex = pageStart + i;
            bitmap[pageIndex / 8] &= ~(1 << (pageIndex % 8)); // Mark as free
        }
    }

    void init(multiboot::Info *mbInfo)
    {
        vga::kprintf("Initializing PMM...\n");

        // Parse memory map to determine total pages

        for (int i = 0; i < mbInfo->mmap_length; i += sizeof(multiboot::MemoryMapEntry))
        {
            multiboot::MemoryMapEntry *mmmt = (multiboot::MemoryMapEntry *)(mbInfo->mmap_addr + i);

            // DEBUG
            // vga::kprintf("Low addr: %x | High Addr: %x | Low Length: %x | High Length: %x | size: %x | Type: %d\n",
            //              mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high, mmmt->size, mmmt->type);
            if (mmmt->type == multiboot::MemoryMapEntry::AVAILABLE)
            {
                // DEBUG
                // vga::kprintf("Memory Length: %d, Page Size: %d, Pages: %d\n",
                //              mmmt->len_low, PAGE_SIZE, mmmt->len_low / PAGE_SIZE);
                totalPages += mmmt->len_low / PAGE_SIZE;
            }
        }

        // Allocate bitmap
        bitmapSize = (totalPages + 7) / 8;                      // Round up to nearest byte
        uint64_t upper_memory_bytes = mbInfo->mem_upper * 1024; // Convert KB to bytes  Upper Memory is the amount of memory above 1MB
        bitmap = (uint8_t *)(upper_memory_bytes - bitmapSize);  // Subtract bitmap size

        for (uint32_t i = 0; i < bitmapSize; i++)
            bitmap[i] = 0xFF; // Initialize all pages as reserved

        // Mark usable memory as free
        for (int i = 0; i < mbInfo->mmap_length; i += sizeof(multiboot::MemoryMapEntry))
        {
            multiboot::MemoryMapEntry *mmmt = (multiboot::MemoryMapEntry *)(mbInfo->mmap_addr + i);
            if (mmmt->type == multiboot::MemoryMapEntry::AVAILABLE)
            {
                markPagesFree(mmmt->addr_low, mmmt->len_low);
            }
        }

        // Reserve bitmap and kernel regions
        markPagesReserved((uint32_t)bitmap, bitmapSize);
        markPagesReserved(0x100000, (uint32_t)mbInfo->mods_addr); // Reserve kernel
        vga::kprintf("PMM initialized with %d pages\n", totalPages);

        // DEBUG
        // vga::kprintf("Total Pages: %d\n", totalPages);
        // vga::kprintf("Bitmap Location: %x\n", bitmap);
        // vga::kprintf("Bitmap Size: %x\n", bitmapSize);
        // vga::kprintf("Upper Memory bytes: %x\n", upper_memory_bytes);
    }

    uint32_t allocPage()
    {
        for (uint32_t i = 0; i < totalPages; i++)
        {
            if (!(bitmap[i / 8] & (1 << (i % 8))))
            {                                    // If the bit is free
                bitmap[i / 8] |= (1 << (i % 8)); // Mark as allocated
                return i * PAGE_SIZE;            // Return the physical address
            }
        }
        vga::kprintf("Out of physical memory!\n");
        return 0; // Out of memory
    }

    void freePage(uint32_t address)
    {
        uint32_t pageIndex = address / PAGE_SIZE;
        bitmap[pageIndex / 8] &= ~(1 << (pageIndex % 8)); // Mark as free
    }
}
