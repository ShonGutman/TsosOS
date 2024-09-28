
# $@ = target file
# $< = first dependency
# $^ = all dependencies

C_SOURCES = $(shell find kernel drivers libc cpu -name '*.c')
HEADERS = $(shell find kernel drivers libc cpu -name '*.c')

# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupts/interrupt.o}

# Change this if your cross-compiler is somewhere else
CC = /usr/bin/gcc
GDB = /usr/bin/gdb
LD = /usr/bin/ld

LD_FLAGS = -m elf_i386
NASM = nasm
NASM_FLAGS_BIN = -f bin
NASM_FLAGS_ELF = -f elf
QEMU = qemu-system-i386
QEMU_FLAGS = -fda
QEMU_DEBUG_FLAGS = -s -S -fda

# -g: Use debugging symbols in gcc
CFLAGS = -m32 -g -ffreestanding -fno-pie -fno-pic -I.

# Default target
all: run


os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} ${LD_FLAGS} -o $@ -T linker.ld $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} ${LD_FLAGS} -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	${QEMU} ${QEMU_FLAGS} os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	${QEMU} ${QEMU_DEBUG_FLAGS} os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${NASM} $< ${NASM_FLAGS_ELF} -o $@

%.bin: %.asm
	${NASM} $< ${NASM_FLAGS_BIN} -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin boot/bootsect.bin *.elf

	find -name '*.o' -type f -delete
	
# Phony targets
.PHONY: all run debug clean