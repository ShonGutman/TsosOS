TSOS Project Log
-----------------

**Platform:** Linux Ubuntu

**Objective:**
	To learn the fundamental concepts of operating systems and implement a basic OS, called TSOS, from scratch.

Requirements
------------

**1.Knowledge of Assembly Language:**

Understand key concepts such as registers, the stack, and basic instructions like `MOV` and `JMP`.

**2.Good Knowledge of the C Language:**

Understand C programming, including pointers, memory management, and data structures, as C is the primary language used to implement the kernel.

**3.Required Tools:**

Make: Used to organize and automate the commands needed for compiling and linking all files.

NASM (Netwide Assembler): The assembler used to produce .bin files.

QEMU: The virtual machine software for running and testing the OS.

GCC: The compiler used to compile C code into object files.

LD: The linker that combines object files into a final executable or binary.

GDB: The debugger for analyzing and debugging the OS at runtime.

**Installation Command:**
	`sudo apt install make nasm qemu-system-x86 gcc gdb binutils`

Tools Overview:
---------------

Make: A build automation tool that manages the compilation and linking processes. It uses a Makefile to define how to compile and link the program.

NASM: An assembler that converts assembly code into machine code. It will be used to generate the binary files necessary for the OS.

QEMU: An open-source emulator and virtualizer that allows you to run the OS in a virtual environment, simulating hardware for testing and development purposes.

GCC: A powerful compiler system supporting various programming languages. In this project, it's used to compile C code into machine code, producing object files (.o) that can be linked to form the OS kernel.

LD: A linker that combines multiple object files (.o) into a single executable or binary file. It is crucial for creating the final bootable image of the OS.

GDB: A debugger that allows you to inspect the inner workings of your program while it runs. You can set breakpoints, step through code, and examine memory, making it invaluable for diagnosing issues in your OS.

