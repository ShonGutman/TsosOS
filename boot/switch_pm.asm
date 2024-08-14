bits 16

;-----------------------------------------------------------------------
; Function: switch_to_pm
; Purpose:  Switch the CPU from real mode (16-bit) to protected mode (32-bit).
;
;
; Parameters:
;   None
;
; Return:
;   None
;
;-----------------------------------------------------------------------
switch_to_pm:
    cli ; Disable interrupts to prevent issues during the switch
    lgdt [gdt_descriptor] ; Load the GDT descriptor into the GDTR register

    ; CR0 (Control Register 0) manipulation
    ; CR0 is a control register that contains various flags for controlling the CPU's operating mode
    mov eax, cr0        ; Load CR0 into EAX - since we can't straight forward modify CRO 
    or eax, 0x1         ; Set the PE (Protection Enable) bit in CR0 (bit 0)
    mov cr0, eax        ; Write the modified value back to CR0, enabling protected mode


    jmp CODE_SEG:init_pm

bits 32

;-----------------------------------------------------------------------
; Function: init_pm
; Purpose:  Initialize the CPU and segments after switching to protected mode.
;
; Parameters:
;   None
;
; Return:
;   None
;
;-----------------------------------------------------------------------
init_pm: ; we are now using 32-bit instructions
    ; After the jump, we're fully in 32-bit protected mode
    ; The CS (Code Segment) register is now loaded with the protected mode segment

    ; Update all the segment registers to use the new protected mode segments
    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up a new stack
    mov ebp, 0x90000 ; Stack base pointer at 0x90000
    mov esp, ebp     ; Stack pointer at the same address (empty stack)

    call BEGIN_PM ; Jump to the main protected mode code