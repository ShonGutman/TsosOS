# TSOS Project Progress

## Date: 14.08.2024

### Overview
This document outlines the progress made in the TSOS project, focusing on the updated files related to disk operations, transitioning to protected mode, handling video output in 32-bit mode, and setting up essential system structures like the Global Descriptor Table (GDT). These updates are critical steps towards creating a functional 32-bit operating system.

### Files Updated

- **disk.asm**:
    - **Purpose**:  Handles disk operations, including reading sectors from the disk. This file is essential for loading the operating system's kernel and other necessary components from the disk into memory.

- **32bit_print.asm**:
    - **Purpose**: Manages screen output in protected mode using direct VGA access, bypassing the BIOS interrupts that are no longer available in this mode. This is crucial for displaying text and information in 32-bit mode.

- **gdt.asm**:
   - **Purpose**: Sets up the Global Descriptor Table (GDT), which is vital for transitioning from real mode to protected mode. The GDT defines memory segments used by the CPU in protected mode, enabling the system to utilize the full 32-bit address space.

- **switch_pm.asm**:
   - **Purpose**: Handles the transition from real mode to protected mode by manipulating the CR0 register and enabling the necessary CPU features. This file is critical for switching the system into 32-bit mode, where advanced memory management and protection features are available.

- **bootsect.asm**:
   - **Purpose**: The initial code executed by the CPU after the BIOS loads the Master Boot Record (MBR). It prepares the system for transitioning to protected mode and passes control to the operating system kernel. This file is crucial for setting up the initial environment and ensuring a smooth transition to the kernel.

## Detailed Explanations and Key Learnings

### 1. **disk.asm**
- **`dl` Driver Number:**  
  The `dl` register contains the drive number provided by the BIOS, which indicates the drive the system booted from. This is essential for ensuring that subsequent disk operations target the correct drive, especially during the loading of the OS kernel.

- **Head, Cylinder, and Sectors Explained:**  
  - **Head:** The component of the disk drive that reads or writes data on the platters.
  - **Cylinder:** A set of tracks aligned vertically across the platters, accessible by the heads.
  - **Sectors:** The smallest data units on a disk, typically containing 512 bytes of data.

- **Errors Can Occur and Possible Errors:**  
  Errors during disk operations might happen due to reasons such as the drive not being ready, encountering bad sectors, or sector not being found. The BIOS interrupt 13h is used to detect and handle these errors, ensuring reliable disk operations.

### 2. **32bit_print.asm**
- **Why Interrupts Can't Be Used Anymore:**  
  In protected mode, the system can no longer rely on BIOS interrupts like `int 0x10` for video output because these interrupts are only available in real mode. This limitation requires direct hardware interaction for tasks such as screen output.

- **What is VGA, Why We Use It, and How to Use It:**  
  VGA (Video Graphics Array) is a video display standard introduced in the late 1980s, providing a way to display text and graphics on the screen. In real mode, VGA services are typically accessed through BIOS interrupts. However, in protected mode, BIOS interrupts are not available, so we need to interact with VGA hardware directly by writing to specific memory addresses.

  **Why We Use VGA:**
  - **Compatibility:** VGA is widely supported and provides a simple interface for text and basic graphics, making it a good choice for early stages of operating system development.
  - **Direct Access:** In protected mode, we can write directly to the video memory mapped at `0xb8000`, allowing us to display characters and colors without relying on the BIOS.

  **How to Use VGA:**
  - **Text Mode:** In text mode, each character on the screen is represented by two bytes in video memory: one byte for the character's ASCII code and another byte for its color attributes. By writing directly to these memory locations, we can control what appears on the screen.
  - **Example:** To display the letter 'A' in white on a black background at the top-left corner of the screen, we would write `0x0F41` to the memory location `0xb8000`. Here, `0x41` is the ASCII code for 'A', and `0x0F` represents the color (white text on a black background).

### 3. **gdt.asm**
- **What is GDT, Why It Is Important in 32-Bit Mode, and How It Works:**  
  The Global Descriptor Table (GDT) is a data structure used by x86 processors to define the characteristics of various memory segments in protected mode. The GDT is essential for transitioning from real mode to protected mode, where it replaces the simpler segment:offset addressing used in real mode with more flexible and powerful segment descriptors.

  **Why It Is Important in 32-Bit Mode:**
  - **Memory Segmentation:** The GDT allows for the definition of memory segments with specific base addresses, limits, and access rights. This is critical in protected mode, where the CPU uses these segments to manage memory protection and access control.
  - **Access Control:** Each segment in the GDT can have its own access rights, allowing the operating system to enforce rules such as read-only or execute-only permissions. This is crucial for system stability and security.
  - **32-Bit Addressing:** In 32-bit mode, the GDT enables the CPU to address the full 4GB of memory space, as opposed to the 1MB limit in real mode.

  **How the GDT Works:**
  - **Segment Descriptors:** Each entry in the GDT is a segment descriptor, which includes fields for the segment's base address, size limit, and access rights. These descriptors tell the CPU how to interpret and manage memory accesses within each segment.
  - **Loading the GDT:** The GDT is loaded into the CPU using the `lgdt` instruction, which takes a pointer to the GDT and its size. Once loaded, the CPU uses the GDT to resolve memory references and enforce access controls.

### 4. **switch_pm.asm**
- **What is the CR0 Register and Why Are We Dealing with It:**  
  The CR0 register is a control register in the CPU that includes various flags to control the processor's operation. One of the most important flags in CR0 is the Protection Enable (PE) bit, which, when set, switches the CPU from real mode to protected mode.

  **Why We Are Dealing with CR0:**
  - **Transition to Protected Mode:** The key to entering protected mode is setting the PE bit in the CR0 register. This action tells the CPU to start using the GDT for memory segmentation and enables the features of protected mode, such as 32-bit addressing and memory protection.
  - **Enabling Advanced Features:** By manipulating CR0, we also gain access to other features of the CPU, such as paging, which is essential for modern operating systems.

### 5. **bootsect.asm**
- **Why We Are Saving BOOT_DRIVE:**  
  Saving the boot drive number from the `dl` register is necessary to retain this information for later use, particularly during the bootloader's operations and when control is passed to the kernel. Knowing which drive the system booted from is critical for loading the correct OS components from the disk.

- **Passing Control to the Kernel in `BEGIN_PM`:**  
  The `BEGIN_PM` function is where the bootloader passes control to the kernel after setting up protected mode. This marks the transition from the bootloader's role to the kernel's, which will take over the system's operation in 32-bit mode. This handoff is crucial for the kernel to start managing the system effectively.
