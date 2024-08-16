
;-----------------------------------------------------------------------
; Function: disk_load
; Purpose:  Load 'dh' sectors from drive 'dl' into ES:BX.
;
; Parameters:
;   dh - number of sectors to read
;   dl - driver number to read data from
;   es:bx - the address of the data to store in the loaded data
;
; Return:
;   None
;-----------------------------------------------------------------------
disk_load:

    pusha
    ; reading from disk requires setting specific values in all registers
    ; so we will overwrite our input parameters from 'dx'. Let's save it
    ; to the stack for later use.
    push dx

    mov ah, 0x02 ; Set ah to BIOS read function (int 13h, function 02h)
    mov al, dh   ; al <- number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02 ; cl <- Sector number to start reading from (0x01 .. 0x11)
                 ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    
    mov ch, 0x00 ; ch <- Cylinder number  (0x0 .. 0x3FF)

    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    
    mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13      ; BIOS interrupt
    jc disk_error ; if error (stored in the carry bit)

    pop dx
    cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne sectors_error
    popa
    ret


disk_error:
    mov si, DISK_ERROR
    call print
    call print_nl
    mov dh, ah ; ah = error code, dl = disk drive that dropped the error
    call print_hex
    jmp disk_loop

sectors_error:
    mov si, SECTORS_ERROR
    call print

disk_loop:
    jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0