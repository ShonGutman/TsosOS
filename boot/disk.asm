
;-----------------------------------------------------------------------
; Function: disk_read
; Purpose:  read sectors from disk
;
; Parameters:
;   ax - LBA address
;   cl - number of sectors to read (up to 128)
;   dl - driver number to read data from
;   es:bx - the address of the data to store in the loaded data
;
; Return:
;   None
;-----------------------------------------------------------------------
disk_read:

    pusha

    push cx             ; temporarily save CL (number of sectors to read)
    call lba_to_chs
    pop ax              ; AL = number of sectors to read

    mov ah, 0x02        ; Set ah to BIOS read function (int 13h, function 02h)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13      ; BIOS interrupt

    jc disk_error       ; if error (stored in the carry bit)

    popa
    ret

disk_error:
    mov si, DISK_ERROR
    call print
    cli             ;diable interrupts, this way CPU cant get out of halt state
    hlt


;-----------------------------------------------------------------------
; Function: lba_to_chs
; Purpose:  convert LBA address format into CHS address format
;
; Parameters:
;   ax - LBA address
;
; Return:
;   cx [bits 0-5]: sector number
;   cx [bits 6-15]: cylinder number
;   dh: head number
;-----------------------------------------------------------------------

lba_to_chs:

    push ax

;   sector number = (LBA % SECTORS_PER_TRACK) + 1
;   head number = (LBA / SECTORS_PER_TRACK) % HEADS
;   cylinder number = (LBA / SECTORS_PER_TRACK) / HEADS

    xor dx, dx                          ; dx = 0
    div word [BPB_SECTORS_PER_TRACK]    ; ax = LBA / SECTORS_PER_TRACK
                                        ; dx = LBA % SECTORS_PER_TRACK
    inc dx                              ; dx = (LBA % SECTORS_PER_TRACK) + 1 = sector number
    mov cx, dx                          ; cx = sector

    xor dx, dx
    div word [BPB_HEADS]                ; ax = (LBA / SECTORS_PER_TRACK) / HEADS = cylinder number
                                        ; dx = (LBA / SECTORS_PER_TRACK) % HEADS = head number
    mov dh, dl
    mov ch, al
    shl ah, 6
    or cl, ah

    pop ax
    ret



DISK_ERROR: db "Disk read error", ENDL, 0