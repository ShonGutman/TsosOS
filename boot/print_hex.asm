;-----------------------------------------------------------------------
; Function: print_hex
; Purpose:  Prints a hex number to the screen.
;
; Parameters:
;   dx - The 16-bit hexadecimal value to print.
;
; Return:
;   None
;-----------------------------------------------------------------------

print_hex:
    pusha                ; Save all registers onto the stack
    xor cx, cx           ; Clear the index register (used to count digits)

    ; Loop through the 4 nibbles of the 16-bit value in DX
hex_loop:
    cmp cx, 4            ; Have we processed 4 digits?
    je print_hex_done    ; If yes, exit the loop
    
    ; 1. Isolate the lowest nibble of DX and convert to ASCII
    mov ax, dx           ; Copy DX to AX (AX is 16-bit, AL is lower 8 bits)
    and ax, 0x000F       ; Mask the higher bits, keep only the lower nibble
    add al, 0x30         ; Convert 0-9 to ASCII ('0' - '9')

    cmp al, 0x39         ; Check if AL > '9'
    jle place_char       ; If AL <= '9', skip to placing the character
    add al, 7            ; Convert 10-15 to ASCII ('A' - 'F')

place_char:
    ; 2. Place the ASCII character into the correct position in HEX_OUT
    mov bx, HEX_OUT + 5  ; Start at the end of HEX_OUT, where the digits go
    sub bx, cx           ; Move to the current character's position
    mov [bx], al         ; Store the character in the correct position

    ; 3. Shift DX right by 4 bits to get the next nibble
    shr dx, 4            ; Shift DX right to bring the next nibble to the lowest bits

    ; 4. Increment the index and continue the loop
    inc cx               ; Increment the index
    jmp hex_loop         ; Repeat the loop for the next nibble

print_hex_done:
    ; Print the resulting hex string using the print function
    mov si, HEX_OUT      ; Load the address of HEX_OUT into BX
    call print           ; Call the print function to display the hex string

    popa                 ; Restore all registers from the stack
    ret                  ; Return from the function

; Memory to store the resulting hex string
HEX_OUT:
    db '0x0000', 0       ; Allocate space for the hex string with null terminator
