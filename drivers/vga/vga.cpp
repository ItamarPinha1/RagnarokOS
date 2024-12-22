#include <stdarg.h>
#include "vga.h"
#include "colors.h"
#include "../ports/ports.h"
#include "../../libc/stdlib/stdlib.h"

void vga::init()
{
    enable_cursor(0, 15);
    clear_screen(true);

    kprintf("Finished Initializing VGA\n");
}

void vga::put_char(char ch, char color)
{
    if (ch == '\n')
    {
        handle_enter();
    }
    else if (ch == '\t')
    {
        handle_tab();
    }
    else if (ch == '\b')
    {
        handle_backspace(color);
    }
    else
    {
        int pos = 2 * (row * WIDTH + col);
        video[pos] = ch;
        video[pos + 1] = color;
        if (changable)
        {
            buffer[perm_row][col] = ch;
        }
        col++;
    }

    if (row < MAX_HEIGHT && changable)
    {
        if (col >= WIDTH)
        {
            if (row >= HEIGHT)
            {
                update_screen(true);
            }

            handle_enter();
        }
    }

    update_cursor(col, row);
}

void vga::clear_screen(bool erase)
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        video[2 * i] = ' ';
        video[2 * i + 1] = BG_BLACK | TEXT_WHITE;
    }

    if (erase)
    {
        for (int i = 0; i < MAX_HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                buffer[i][j] = '\0';
            }
        }
        perm_row = 0;
    }
    row = 0;
    col = 0;
    update_cursor(col, row);
}

void vga::kprintf(const char *text, ...)
{
    va_list args;
    va_start(args, text);

    char intInString[12];
    char hexInString[12];

    while (*text)
    {
        if (*text == '%')
        {
            text++;
            switch (*text)
            {
            case 's': // string
            {
                char *str = va_arg(args, char *);
                puts(str);
                break;
            }
            case 'c': // char
            {
                char ch = (char)va_arg(args, int);
                put_char(ch, BG_BLACK | TEXT_WHITE);
                break;
            }
            case 'd': // int
            {
                itoa(va_arg(args, int), intInString, 10);
                puts(intInString);

                break;
            }
            case 'x': // lowercase hex
                itoa(va_arg(args, int), hexInString, 16);
                puts("0x");
                puts(hexInString);
                break;
            case 'X': // uppercase hex
            {
                itoa(va_arg(args, int), hexInString, 16);

                // Convert to uppercase
                for (int i = 0; hexInString[i] != '\0'; i++)
                {
                    if (hexInString[i] >= 'a' && hexInString[i] <= 'f')
                    {
                        hexInString[i] -= 32; // Convert to uppercase
                    }
                }

                puts("0x");
                puts(hexInString);
                break;
            }
            default:
                put_char('%', BG_BLACK | TEXT_WHITE);
                put_char(*text, BG_BLACK | TEXT_WHITE);
                break;
            }
        }
        else
        {
            put_char(*text, BG_BLACK | TEXT_WHITE);
        }
        text++;
    }

    va_end(args);
}

void vga::update_screen(bool lower)
{
    int temp_row = 0;
    int temp_col = 0;
    if (lower)
    {
        temp_row = row;
        temp_col = col;
        clear_screen(false);
        offset++;
        changable = false;
        for (int i = 0 + offset; i < HEIGHT + offset; i++)
        {
            kprintf("%s\n", buffer[i]);
        }
        changable = true;
        row = temp_row - 1;
        col = temp_col;
    }

    else
    {
        if (offset <= 0)
        {
            return;
        }
        temp_row = row;
        temp_col = col;
        clear_screen(false);
        offset--;
        changable = false;
        for (int i = 0 + offset; i < HEIGHT + offset; i++)
        {
            kprintf("%s\n", buffer[i]);
        }
        changable = true;
        row = temp_row + 1;
        col = temp_col;
    }

    update_cursor(col, row);
}

void vga::puts(char *str)
{
    while (*str)
    {
        put_char(*str++, BG_BLACK | TEXT_WHITE);
    }
}

void vga::enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga::update_cursor(uint32_t col, uint32_t row)
{
    uint16_t pos = row * WIDTH + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

uint16_t vga::get_cursor_position()
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void vga::handle_enter()
{
    row++;
    col = 0;

    if (changable)
    {
        perm_row++;

        if (row >= HEIGHT)
        {
            update_screen(true);
        }
    }
}

void vga::handle_tab()
{
    if (changable)
    {
        for (int i = 0; i < 4; i++)
        {
            buffer[perm_row][col++] = ' ';
        }
    }
}

void vga::handle_backspace(char color)
{
    bool auto_del = false;

    if ((row == 0 && col == 0) || row < 0)
    {
        return;
    }

    else if (col == 0)
    {
        row--;
        if (changable)
        {
            perm_row--;
        }
        col = WIDTH - 1;

        int pos = 2 * (row * WIDTH + col);
        if (video[pos] == ' ')
        {
            auto_del = true;
            while (video[pos] == ' ')
            {
                col--;
                pos = 2 * (row * WIDTH + col);
                if (col == -1)
                {
                    break;
                }
            }
            col++;
        }
    }

    else
    {
        col--;
    }

    if (!auto_del)
    {
        int pos = 2 * (row * WIDTH + col);
        video[pos] = ' ';
        video[pos + 1] = color;
        if (changable)
        {
            buffer[perm_row][col] = ' ';
        }
    }
}