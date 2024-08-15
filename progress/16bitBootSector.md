
# TSOS Project Progress

## Date: 11.08.2024

### Overview

This document captures the recent advancements made in the TSOS project, focusing on critical aspects of boot sector programming. The topics covered include the significance of the boot signature, memory origin setup, sector padding.

### Files Updated

- **bootsect.asm**: 
  - **Purpose**: This file contains the initial boot code loaded by the BIOS, setting up the environment for further execution in real mode.
  - **Key Additions**:
    - Boot signature (`dw 0xaa55`)
    - Memory origin directive (`org 0x7C00`)
    - Sector padding (`times 510-($-$$) db 0`)

- **print.asm**: 
  - **Purpose**: Provides functions for displaying text on the screen using BIOS interrupts, essential for early-stage debugging and output.

- **print_hex.asm**: 
  - **Purpose**: Provides functions for displaying hex values on the screen using BIOS interrupts, essential for early-stage debugging and output.

### Detailed Explanations and Key Learnings

#### 1. **Boot Signature - `0xAA55`**

- **Explanation**:
  - The line `dw 0xaa55` is a critical part of any boot sector. The BIOS requires that the last two bytes of the first sector (512 bytes) contain this specific value. It acts as a boot signature or magic number that the BIOS uses to verify that the sector is bootable.
  - **Why It’s Needed**: 
    - When the system starts, the BIOS reads the first sector of the bootable device into memory at `0x7C00` and checks the last two bytes. If these bytes are not `0xAA55`, the BIOS assumes that the sector is not a valid boot sector and will not proceed to load it. This safeguard ensures that only sectors intended to be bootable are executed.

#### 2. **Memory Origin Directive - `org 0x7C00`**

- **Explanation**:
  - The `org 0x7C00` directive instructs the assembler to assume that the code will be loaded at the memory address `0x7C00`. This is the location where the BIOS loads the boot sector into memory.
  - **Why It’s Used**:
    - When the assembler generates machine code, it needs to know the memory address where the code will reside to correctly calculate addresses for jumps and data access. Since the boot sector is loaded at `0x7C00`, this directive ensures that all addresses are calculated relative to this origin.
    - Without this, any hardcoded addresses in the boot sector code would be incorrect, leading to potential failures when the code tries to access memory or jump to other parts of the program.

#### 3. **Sector Padding - `times 510-($-$$) db 0`**

- **Explanation**:
  - The line `times 510-($-$$) db 0` pads the boot sector with zeros up to 510 bytes. The `$` symbol represents the current address, and `$$` represents the start of the section. Therefore, `($-$$)` gives the size of the code so far, and `510-($-$$)` calculates the number of bytes needed to fill up the sector to 510 bytes.
  - **Why It’s Done**:
    - A boot sector must be exactly 512 bytes in size. The first 510 bytes are used for code and data, and the last two bytes must be the boot signature (`0xAA55`). If the code and data take up less than 510 bytes, the remaining space is filled with zeros to ensure the sector size requirement is met.
    - This padding ensures that the boot signature is correctly placed at the end of the sector, which is critical for the BIOS to recognize the boot sector as valid.
