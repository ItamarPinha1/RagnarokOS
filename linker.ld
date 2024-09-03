ENTRY(_start)

SECTIONS
{
    /* Place the code (.text) at 0x1000 */
    . = 0x1000;

    .text : {
        *(.text)
    }

    /* Place the data (.data) right after the code */
    .data : {
        *(.data)
    }

    /* Place the uninitialized data (.bss) after .data */
    .bss : {
        *(.bss)
    }

    /* Define the end of the kernel */
    _end = .;
}