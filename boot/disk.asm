disk_load:
    pusha
    push dx

    mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = 'read'
    mov al, dh   ; al <- number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02 ; cl <- sector (0x01 .. 0x11)
                 ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)

    int 0x13 
    jc disk_error ; if error (stored in the carry bit)

    pop dx
    cmp al, dh    ; BIOS also sets 'al' to the # of sectors read.
    jne sectors_error
    popa
    ret


disk_error:
    jmp disk_loop

sectors_error:

disk_loop:
    jmp $

