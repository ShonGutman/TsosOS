TSOS Project Log
-----------------

**Platform:** Linux Ubuntu

**Objective:**
	To learn the fundamental concepts of operating systems and implement a basic OS, called TSOS, from scratch.

Requirements
------------

**1.Knowledge of Assembly Language:**

Understand key concepts such as registers, the stack, and basic instructions like `MOV` and `JMP`.

**2.Required Tools:**

Make: Used to organize and automate the commands needed for compiling and linking all files.

NASM (Netwide Assembler): The assembler used to produce .bin files.

QEMU: The virtual machine software for running and testing the OS.

**Installation Command:**
	`sudo apt install make nasm qemu-system-x86`

Tools Overview:
---------------

Make: A build automation tool that manages the compilation and linking processes. It uses a Makefile to define how to compile and link the program.

NASM: An assembler that converts assembly code into machine code. It will be used to generate the binary files necessary for the OS.

QEMU: An open-source emulator and virtualizer that allows you to run the OS in a virtual environment, simulating hardware for testing and development purposes.