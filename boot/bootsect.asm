bits 32

MAGIC_NUMBER equ 0x1badb002 ; magic number for GRUB
FLAGS equ 0x0 ; multiboot flags
CHECKSUM equ -(MAGIC_NUMBER + FLAGS) ; checksum + flags + magic_number should be 0

KERNEL_STACK_SIZE equ 8192      ; size of stack in bytes

global start
extern main

section .text
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

start:
    cli
    mov esp, stack_space
    call main
    hlt
hltKenrel:
    cli
    hlt
    jmp hltKenrel

section .bss
resb KERNEL_STACK_SIZE
stack_space: