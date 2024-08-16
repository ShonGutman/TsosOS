;-----------------------------------------------------------------------
; GDT (Global Descriptor Table):
;
; The GDT defines the characteristics of the various memory areas used in
; your program. It specifies base addresses, segment limits, and access
; control information for segments.
;
;
;   Code Segment Descriptor:
;   ------------------------
;   
;   Base (32 bit) - Start address of the segment (in our case 0x00)
;   Limit (20 bit) - Length of segment (in our case 0xfffff, which is the maximum)
;   Present (1 bit) - 1 for used segments (in our case - 1)
;   Privilege (2 bit) - help define segment protection (possible options: 00 | 01 | 10 | 11 . in our case - 00)
;   Type (1 bit) - 1 if Code or Data segment (in our case - 1)
;
;   Flags:
;   
;     1. Type flags (4 bit):
;       IN our case will be: 1010
;
;       - Executable: Contain Code? Yes, thus: 1
;       - Conforming: can be executed from lower privilege segments? No, thus: 0
;       - Readable: is it only executable or also readable? Yes, thus: 1
;       - Accessed: used by CPU. set to 1 if when CPU uses the segment. Set to 0 at let CPU deal with it
;
;     2. Other flags:
;       IN our case will be: 1100
;
;       - Granularity: if 1 then limit *= 0x1000 (to expand segment - we want it)
;       - 32 bit? Yes, thus: 1
;       - Long: for 64-bit so set to 0
;       - Available: For software use, not used by hardware. set to 0
;
;
;   Data Segment Descriptor
;   -----------------------
;
;   No changes to - Base, Limit, Present, Privilege, Type, Other flags
;
;   Changes in Type flags:
;       IN our case will be: 0010
;
;       - Executable: Contain Code? No, thus: 0
;       - Direction: segment grows downwords (We dont want that so set to 0)
;       - Writable: 0 if read only (int our case it is not so set to 1)
;       - Accessed: same as in Code segment
;
;--------------------------------------------------------------------------


gdt_start:
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
gdt_code: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; Present, Privilege, Type and Type flags (8 bits)
    db 11001111b ; Other flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
gdt_data:
    dw 0xffff       ; segment length, bits 0-15
    dw 0x0          ; segment base, bits 0-15
    db 0x0          ; segment base, bits 16-23
    db 10010010b    ; Present, Privilege, Type and Type flags (8 bits)
    db 11001111b    ; Other flags (4 bits) + segment length, bits 16-19
    db 0x0          ; segment base, bits 24-31

gdt_end:

; GDT descriptor (to be loaded by the LGDT instruction)
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)

; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start