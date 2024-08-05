; A simple boot sector program that loops forever

; The BIOS loads the MBR to location 0x7C00 in memory.
; We need to tell the assembler to calculate all addresses with this offset.
org 0x7C00

; The OS is first launched in real mode (which is 16-bit mode).
bits 16

; Infinite loop
start:
    jmp start   ; Jump to the current address to create an infinite loop

; Fill the rest of the sector with zeros (510 bytes total)
times 510-($-$$) db 0

; Boot signature (magic number) to indicate that this is executable code
dw 0xaa55
