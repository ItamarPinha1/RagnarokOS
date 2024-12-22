#ifndef MULTIBOOT_HEADER_HPP
#define MULTIBOOT_HEADER_HPP

#include <stdint.h>

namespace multiboot
{
    // Constants for Multiboot
    constexpr uint32_t HEADER_MAGIC = 0x1BADB002;
    constexpr uint32_t BOOTLOADER_MAGIC = 0x2BADB002;

    // Multiboot Header Flags
    constexpr uint32_t PAGE_ALIGN = 0x00000001;
    constexpr uint32_t MEMORY_INFO = 0x00000002;
    constexpr uint32_t VIDEO_MODE = 0x00000004;
    constexpr uint32_t AOUT_KLUDGE = 0x00010000;

    // Multiboot Info Flags
    constexpr uint32_t INFO_MEMORY = 0x00000001;
    constexpr uint32_t INFO_BOOTDEV = 0x00000002;
    constexpr uint32_t INFO_CMDLINE = 0x00000004;
    constexpr uint32_t INFO_MODS = 0x00000008;
    constexpr uint32_t INFO_AOUT_SYMS = 0x00000010;
    constexpr uint32_t INFO_ELF_SHDR = 0x00000020;
    constexpr uint32_t INFO_MEM_MAP = 0x00000040;
    constexpr uint32_t INFO_DRIVE_INFO = 0x00000080;
    constexpr uint32_t INFO_CONFIG_TABLE = 0x00000100;
    constexpr uint32_t INFO_BOOT_LOADER_NAME = 0x00000200;
    constexpr uint32_t INFO_APM_TABLE = 0x00000400;
    constexpr uint32_t INFO_VBE_INFO = 0x00000800;
    constexpr uint32_t INFO_FRAMEBUFFER_INFO = 0x00001000;

    // Multiboot Header
    struct Header
    {
        uint32_t magic;
        uint32_t flags;
        uint32_t checksum;
        uint32_t header_addr;
        uint32_t load_addr;
        uint32_t load_end_addr;
        uint32_t bss_end_addr;
        uint32_t entry_addr;
        uint32_t mode_type;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    };

    // AOUT Symbol Table
    struct AOutSymbolTable
    {
        uint32_t tabsize;
        uint32_t strsize;
        uint32_t addr;
        uint32_t reserved;
    };

    // ELF Section Header Table
    struct ELFSectionHeaderTable
    {
        uint32_t num;
        uint32_t size;
        uint32_t addr;
        uint32_t shndx;
    };

    // Multiboot Info
    struct Info
    {
        uint32_t flags;
        uint32_t mem_lower;
        uint32_t mem_upper;
        uint32_t boot_device;
        uint32_t cmdline;
        uint32_t mods_count;
        uint32_t mods_addr;

        union
        {
            AOutSymbolTable aout_sym;
            ELFSectionHeaderTable elf_sec;
        } u;

        uint32_t mmap_length;
        uint32_t mmap_addr;
        uint32_t drives_length;
        uint32_t drives_addr;
        uint32_t config_table;
        uint32_t boot_loader_name;
        uint32_t apm_table;
        uint32_t vbe_control_info;
        uint32_t vbe_mode_info;
        uint16_t vbe_mode;
        uint16_t vbe_interface_seg;
        uint16_t vbe_interface_off;
        uint16_t vbe_interface_len;
        uint64_t framebuffer_addr;
        uint32_t framebuffer_pitch;
        uint32_t framebuffer_width;
        uint32_t framebuffer_height;
        uint8_t framebuffer_bpp;
        uint8_t framebuffer_type;

        union
        {
            struct
            {
                uint32_t framebuffer_palette_addr;
                uint16_t framebuffer_palette_num_colors;
            };
            struct
            {
                uint8_t framebuffer_red_field_position;
                uint8_t framebuffer_red_mask_size;
                uint8_t framebuffer_green_field_position;
                uint8_t framebuffer_green_mask_size;
                uint8_t framebuffer_blue_field_position;
                uint8_t framebuffer_blue_mask_size;
            };
        };
    };

    // Memory Map Entry
    struct MemoryMapEntry
    {
        uint32_t size;
        uint32_t addr_low;
        uint32_t addr_high;
        uint32_t len_low;
        uint32_t len_high;
        uint32_t type;

        // Memory Types
        static constexpr uint32_t AVAILABLE = 1;
        static constexpr uint32_t RESERVED = 2;
        static constexpr uint32_t ACPI_RECLAIMABLE = 3;
        static constexpr uint32_t NVS = 4;
        static constexpr uint32_t BADRAM = 5;
    };

    // Multiboot Module
    struct Module
    {
        uint32_t mod_start;
        uint32_t mod_end;
        uint32_t cmdline;
        uint32_t pad;
    };

    // APM BIOS Info
    struct APMInfo
    {
        uint16_t version;
        uint16_t cseg;
        uint32_t offset;
        uint16_t cseg_16;
        uint16_t dseg;
        uint16_t flags;
        uint16_t cseg_len;
        uint16_t cseg_16_len;
        uint16_t dseg_len;
    };

} // namespace multiboot

#endif // MULTIBOOT_HEADER_HPP
