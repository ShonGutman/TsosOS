# Variables
NASM = nasm
NASM_FLAGS = -f bin
QEMU = qemu-system-x86_64

# Detect the .asm file automatically
ASM = $(wildcard *.asm)
BIN = $(ASM:.asm=.bin)

# Default target
all: run

# Rule to create .bin file from .asm file
%.bin: %.asm
	$(NASM) $(NASM_FLAGS) $< -o $@

# Rule to run .bin file in QEMU
run: $(BIN)
	$(QEMU) $(BIN)

# Clean up generated files
clean:
	rm -f *.bin

# Phony targets
.PHONY: all run clean

