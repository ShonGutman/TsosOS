# Variables
NASM = nasm
NASM_FLAGS = -f bin
QEMU = qemu-system-x86_64
QEMU_FLAGS = -fda

# Default target
all: run

# Rule to create .bin file from .asm file
boot/bootsect.bin: boot/bootsect.asm
	$(NASM) $(NASM_FLAGS) $< -o $@

# Rule to run .bin file in QEMU
run: boot/bootsect.bin
	$(QEMU) $(QEMU_FLAGS) $<

# Clean up generated files
clean:
	rm -f boot/*.bin

# Phony targets
.PHONY: all run clean

