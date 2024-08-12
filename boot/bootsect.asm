
; The BIOS loads the MBR to location 0x7C00 in memory.
; We need to tell the assembler to calculate all addresses with this offset.
org 0x7C00

; The OS is first launched in real mode (which is 16-bit mode).
bits 16

; Infinite loop
start:

    ; setup data segments
    xor ax, ax  ; can't set ds/es directly
    mov ds, ax
    mov es, ax

    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

    ; setup stack
    mov ss, ax
    mov bp, 0x9000 ; stack grows downwards. this is an address far away from 0x7c00 so that we don't get overwritten

    mov sp, bp ; if the stack is empty then sp points to bp

    ; print message to screen
    mov si, MSG_REAL_MODE
    call print
    call print_nl


    jmp $   ; Jump to the current address to create an infinite loop


%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/32bit_print.asm"
%include "boot/gdt.asm"

BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0

; Fill the rest of the sector with zeros (510 bytes total)
times 510-($-$$) db 0

; Boot signature (magic number) to indicate that this is executable code
dw 0xaa55
