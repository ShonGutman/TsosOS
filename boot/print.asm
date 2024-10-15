;-----------------------------------------------------------------------
; Function: print
; Purpose:  Prints a null-terminated string to the screen.
;
; Parameters:
;   ds:si - Points to the beginning of the string to print.
;
; Return:
;   None
;-----------------------------------------------------------------------

print:
    pusha                ; Save all registers onto the stack

print_loop:
    lodsb                ; Load byte at DS:SI into AL, and increment SI
    or al, al            ; Set flags based on AL (check if AL is 0)
    jz print_done        ; If AL is 0 (null terminator), jump to done

    ; BIOS interrupt to print character in AL
    mov ah, 0x0e         
    int 0x10             

    jmp print_loop       ; Continue printing the next character

print_done:
    popa                 ; Restore all registers from the stack
    ret                                
