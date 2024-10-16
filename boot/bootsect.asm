
; The BIOS loads the MBR to location 0x7C00 in memory.
; We need to tell the assembler to calculate all addresses with this offset.
org 0x7C00

; The OS is first launched in real mode (which is 16-bit mode).
bits 16

KERNEL_OFFSET equ 0x2000
KERNEL_SEGMENT equ 0x07c0
%define ENDL 0x0d, 0x0a

;credit: https://wiki.osdev.org/FAT
jmp short start
nop

BPB_OEM db 'MSWIN4.1'
BPB_BYTES_PER_SECTOR dw 512
BPB_SECTORS_PER_CLUSTER db 1
BPB_RESERVED_SECTORS dw 1
BPB_FILE_ALLOCATION_TABLES db 2
BPB_ROOT_DIR_ENTIRES dw 0x00e0
BPB_TOTAL_SECTORS dw 2880           ; 2880 * 512 = 1.44 MB
BPB_MEDIA_DESCRIPTOR_TYPE db 0xf0   ; 3.5 inch Floopy Disk
BPB_SECTORS_PER_FAT dw 9
BPB_SECTORS_PER_TRACK dw 18
BPB_HEADS dw 2
BPB_HIDDEN_SECOTRS dd 0
BPB_LARGE_SECTOR_COUNT dd 0

;Extended:
EBPB_DRIVER_NUMBER db 0
EBPB_RESERVED db 0
EBPB_SIGNATURE db 0x29
EBPB_VOLUME_ID dd 0x12345678        ; Serial number, can be anything
EBPB_VOLUME_LABLE db 'TSOS OS    '  ; 11 chars, padded with spaces
EBPB_SYSTEM_ID db 'FAT12   '        ; 8 chars, padded with spaces

halt:
    cli             ;diable interrupts, this way CPU cant get out of halt state
    hlt
start:

    ; setup data segments
    xor ax, ax  ; can't set ds/es directly
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7c00 ; stack grows downwards. Thus, it won't overwrite ower bootloader

    mov [EBPB_DRIVER_NUMBER], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

    mov si, LOADING_KERNEL
    call print

    call load_kernel ; read the kernel from disk
    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'


bits 16

load_kernel:

    ;LBA of Root directory = reserved + fatCount * sectors_per_fat
    mov ax, [BPB_SECTORS_PER_FAT]
    mov bl, [BPB_FILE_ALLOCATION_TABLES]
    xor bh, bh
    mul bx                                  ; ax = fatCount * sectors_per_fat
    add ax, [BPB_RESERVED_SECTORS]          ; ax = LBA of Root directory
    push ax                                 ; preserve data into stack

    mov ax, [BPB_ROOT_DIR_ENTIRES]
    shl ax, 5                               ; ax *= 32
    xor dx, dx
    div word [BPB_BYTES_PER_SECTOR]         ; root_dir_sectors = (num_of_entries * 32) / bytes_per_sector

    or dx, dx
    jz root_dir_after                       ; if dx != 0 (there is a remainder), add 1 to ax
    inc ax                                  ; We add 1 to fit the data, this Sector is partially filled with entries

root_dir_after:

    ;Read Root directory
    mov cl, al                              ; number of sectors to read = size of root directory
    pop ax                                  ; LBA of root directory
    mov dl, [EBPB_DRIVER_NUMBER]
    mov bx, buffer

    call disk_read

    ;Save data lba for later use
    xor ch, ch
    add ax, cx
    mov [data_lba], ax

    ; Search for kernel.bin
    xor bx, bx                  ; will be used to count directories we already checked
    mov di, buffer

serach_kernel_loop:

    mov si, KERNEL_NAME
    mov cx, 11                  ; Length of file
    push di
    repe cmpsb                  ; Compares 2 strings in si, di until cx = 0
    pop di
    je found_kernel

    add di, 32                  ; Move to next directory
    inc bx
    cmp bx, [BPB_ROOT_DIR_ENTIRES]
    jl serach_kernel_loop

    ; Kernel not found
    mov si, KERNEL_NOT_FOUND
    call print
    jmp halt


found_kernel:

    ; di should still have the address of the kernel entry
    mov ax, [di + 26]   ; offset of low cluster field
    mov [kernel_cluster], ax

    ; load FAT from disk into memory
    mov cl, [BPB_SECTORS_PER_FAT]
    mov ax, [BPB_RESERVED_SECTORS]
    mov dl, [EBPB_DRIVER_NUMBER]
    mov bx, buffer

    call disk_read

    ; Read Kernel
    mov bx, KERNEL_SEGMENT
    mov es, bx
    mov bx, KERNEL_OFFSET

load_kernel_loop:

    ; cluster_lba = (kernel_cluster - 2) * sectors_per_cluster + start_sector

    mov ax, [kernel_cluster]
    sub ax, 2                       ; first 2 clusters are reserved

    mov cl, [BPB_SECTORS_PER_CLUSTER]
    xor ch, ch
    mul cx                                  ; ax = (kernel_cluster - 2) * sectors_per_cluster

    add ax, [data_lba]

    mov cl, 1
    mov dl, [EBPB_DRIVER_NUMBER]
    call disk_read

    ; !note! : if Kernel size is bigger then 65 KB this would overflow and an inc of es would be required
    ; for the time being this is not going to be chnaged
    add bx, [BPB_BYTES_PER_SECTOR]

    mov ax, [kernel_cluster]
    mov cx, 3
    mul cx
    mov cx, 2
    div cx                  ; ax = index of entry in FAT. since every entry is 12 bit we need to mul by 1.5


    mov si, buffer
    add si, ax
    mov ax, [ds:si]         ; Read entry from FAT table at RAM

    or dx, dx               ; check the remainder is 0
    jz even

odd:
    shr ax, 4                   ; if odd take first 12 bits
    jmp next_cluster
even:
    and ax, 0x0fff              ; if even take last 12 bits

next_cluster:

    cmp ax, 0x0ff8              ; end of FAT chain
    jae read_finish

    mov [kernel_cluster], ax
    jmp load_kernel_loop

read_finish:
    ret


bits 32
BEGIN_PM:
    call 0x9c00; Give control to the kernel
    
    jmp $ ; Stay here when the Kernel returns control to us (it should never happen)
  
%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

data_lba: dw 0
kernel_cluster: dw 0
LOADING_KERNEL db "Loading Kernel", ENDL, 0
KERNEL_NOT_FOUND db "kernel not found", ENDL, 0
KERNEL_NAME db "KERNEL  BIN"

; Fill the rest of the sector with zeros (510 bytes total)
times 510-($-$$) db 0

; Boot signature (magic number) to indicate that this is executable code
dw 0xaa55

buffer:
