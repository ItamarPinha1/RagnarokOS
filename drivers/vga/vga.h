#pragma once
#include <stdint.h>

namespace vga
{
    static const int MAX_HEIGHT = 1000;
    static const int WIDTH = 80;
    static const int HEIGHT = 25;
    static char *video = (char *)0xC00B8000;

    void init();
    void put_char(char ch, char color);
    void clear_screen(bool erase);
    void kprintf(const char *text, ...);
    void update_screen(bool lower);

    void handle_enter();
    void handle_tab();
    void handle_backspace(char color);

    uint16_t get_cursor_position();
    void update_cursor(uint32_t col, uint32_t row);
    void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
    void puts(char *str);

    static char buffer[MAX_HEIGHT][WIDTH];
    static int offset = 0;
    static bool changable = true;
    static int perm_row = 0;
    static int row = 0;
    static int col = 0;
};
