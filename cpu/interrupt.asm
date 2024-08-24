; Source: https://wiki.osdev.org/Interrupts
;
; Interrupt Cases:
; ------------------
;
; 1. Exceptions:
;    -----------
;    - Generated internally by the CPU to alert the kernel of critical events.
;    - Examples include Double Fault, Page Fault, and General Protection Fault.
;
; 2. Interrupt Request (IRQ) or Hardware Interrupt:
;    ----------------------------------------------
;    - Generated by hardware devices.
;    - Managed by the Master and Slave PICs and then handed to the CPU.
;
; 3. Software Interrupt:
;    -------------------
;    - Triggered by software running on the CPU to request kernel attention.
;    - Commonly used for System Calls.
;
; Note:
; -----
; - It is common practice to reserve the first 32 interrupt vectors for exceptions, as per Intel's recommendation.
; - The partitioning of the remaining vectors is left to the developer's discretion.
;







%macro ISR_NO_ERRCODE 1
    global isr%1
    isr%1:
        cli
        push byte 0 ; error code is zero
        push byte %1 ; ISR number
        jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli
        ;error code has already been pushed
        push byte %1 ; ISR number
        jmp isr_common_stub
%endmacro


ISR_NO_ERRCODE 0
ISR_NO_ERRCODE 1
ISR_NO_ERRCODE 2
ISR_NO_ERRCODE 3
ISR_NO_ERRCODE 4
ISR_NO_ERRCODE 5
ISR_NO_ERRCODE 6
ISR_NO_ERRCODE 7
ISR_ERRCODE 8
ISR_NO_ERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NO_ERRCODE 15
ISR_NO_ERRCODE 16
ISR_NO_ERRCODE 17
ISR_NO_ERRCODE 18
ISR_NO_ERRCODE 19
ISR_NO_ERRCODE 20
ISR_NO_ERRCODE 21
ISR_NO_ERRCODE 22
ISR_NO_ERRCODE 23
ISR_NO_ERRCODE 24
ISR_NO_ERRCODE 25
ISR_NO_ERRCODE 26
ISR_NO_ERRCODE 27
ISR_NO_ERRCODE 28
ISR_NO_ERRCODE 29
ISR_NO_ERRCODE 30
ISR_NO_ERRCODE 31


extern isr_handler

isr_common_stub:
    ;save CPU state
    pusha ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds ; lower 16-bits of eax = ds
    push eax ; save data segment descriptor

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; call C function
    call isr_handler

    ; Restore state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa

    add esp, 8 ; clear pushed error code and pushed ISR number
    sti
    iret ;pops 5 things at once: CS, EIP, EFLAGS, SS, ESP

