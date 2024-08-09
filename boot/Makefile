# Variables
NASM = nasm
NASM_FLAGS = -f bin
QEMU = qemu-system-x86_64

# Default target
all: run

# Rule to create .bin file from .asm file
bootsect.bin: bootsect.asm
	$(NASM) $(NASM_FLAGS) $< -o $@

# Rule to run .bin file in QEMU
run: bootsect.bin
	$(QEMU) $<

# Clean up generated files
clean:
	rm -f *.bin

# Phony targets
.PHONY: all run clean

