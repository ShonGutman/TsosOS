[bits 32] ; Using 32-bit protected mode

; Constants
VIDEO_MEMORY equ 0xb8000           ; Video memory base address
WHITE_ON_BLACK equ 0x0f            ; The color byte for white text on black background

;-----------------------------------------------------------------------
; Function: print_string_pm
; Purpose:  Prints a null-terminated string to the screen in 32-bit mode.
;
; Parameters:
;   esi - Pointer to the string to be printed.
;
; Return:
;   None
;-----------------------------------------------------------------------

print_string_pm:
    pusha                         

    mov edx, VIDEO_MEMORY         

print_string_pm_loop:

    lodsb                         ; Load byte at ESI into AL, and increment ESI
    or al, al                     ; Set flags based on AL (check if AL is 0)
    jz print_string_pm_done       ; If AL is 0 (null terminator), jump to done

    mov ah, WHITE_ON_BLACK        ; Set the color attribute (white on black)
    mov [edx], ax                 ; Store the character and attribute in video memory
    add edx, 2                    ; Move to the next character position in video memory

    jmp print_string_pm_loop      ; Repeat the loop for the next character

print_string_pm_done:
    popa                          
    ret                           
