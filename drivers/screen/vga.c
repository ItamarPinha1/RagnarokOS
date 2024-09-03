#include <stdarg.h>
#include "vga.h"
#include "colors.h"

#define WIDTH 80
#define HEIGHT 25

char *video = (char *)0xB8000;
int row = 0;
int col = 0;

void putChar(char ch, char color)
{
    if (ch == '\n')
    {
        row++;
        col = 0;
    }
    else if (ch == '\t')
    {
        col += 4;
    }
    else
    {
        int pos = 2 * (row * WIDTH + col);
        video[pos] = ch;
        video[pos + 1] = color;
        col++;
    }

    if (col >= WIDTH)
    {
        col = 0;
        row++;
    }
}

void ClearScreen()
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        video[2 * i] = ' ';
        video[2 * i + 1] = BG_BLACK | TEXT_WHITE;
    }
    row = 0;
    col = 0;
}

void printf(char *text, ...)
{
    va_list args;
    va_start(args, text);

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
                while (*str)
                {
                    putChar(*str++, BG_BLACK | TEXT_WHITE);
                }
                break;
            }
            case 'c': // char
            {
                char ch = (char)va_arg(args, int);
                putChar(ch, BG_BLACK | TEXT_WHITE);
                break;
            }
            case 'd': // int
            {
                // Need to implement
                break;
            }
            case 'f': // float
            {
                // Need to implement
                break;
            }
            default:
                putChar('%', BG_BLACK | TEXT_WHITE);
                putChar(*text, BG_BLACK | TEXT_WHITE);
                break;
            }
        }
        else
        {
            putChar(*text, BG_BLACK | TEXT_WHITE);
        }
        text++;
    }

    va_end(args);
}
