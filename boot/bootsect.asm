
; The BIOS loads the MBR to location 0x7C00 in memory.
; We need to tell the assembler to calculate all addresses with this offset.
org 0x7C00

KERNEL_OFFSET equ 0x1000 ; We will use the same when linking the kernel

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

    call load_kernel ; read the kernel from disk
    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'

    jmp $   ; Never executed


bits 16
load_kernel:
    mov si, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 16 ; Our future kernel will be larger, make this big
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret



bits 32
BEGIN_PM:

    mov esi, MSG_PROTECTED_MODE
    call print_string_pm

    call KERNEL_OFFSET ; Give control to the kernel

    jmp $ ; Stay here when the kernel returns control to us (it should never happen)

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/32bit_print.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROTECTED_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

; Fill the rest of the sector with zeros (510 bytes total)
times 510-($-$$) db 0

; Boot signature (magic number) to indicate that this is executable code
dw 0xaa55
