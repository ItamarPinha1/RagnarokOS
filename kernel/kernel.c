#include "../drivers/screen/vga.h"

void main()
{
    ClearScreen();

    char *name = "Itamar";

    printf("My name %%f is %c and %c and for last im %s abc", 'a', 'b', name);
}