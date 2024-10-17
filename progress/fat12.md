
# TSOS Project Progress

## Date: 17.10.2024

### Overview
This document outlines the recent additions to the bootloader, specifically regarding FAT12 support. It covers core concepts like Logical Block Addressing (LBA) and its conversion to Cylinder-Head-Sector (CHS), the FAT12 headers in the boot sector, the FAT12 memory layout, and the process for finding and loading the kernel code. This is crucial for understanding how the bootloader interacts with the file system to load the operating system.

### 1. **What is LBA and How Does Reading from Disk Work?**
- **Logical Block Addressing (LBA)**:
    - LBA is a scheme used to specify the location of blocks of data on a disk without using traditional Cylinder-Head-Sector (CHS) addressing. In LBA, each block is given a unique number, starting from zero.
    - The process of reading from a disk involves accessing sectors, heads, and cylinders in sequence:
        - **Sector**: The smallest unit of data storage, typically 512 bytes.
        - **Head**: Each platter of the hard disk has two surfaces (top and bottom), and a read/write head is assigned to each.
        - **Cylinder**: A set of tracks located at the same position on all the platters of the disk.
    - **Converting LBA to CHS**: 
        - To convert an LBA address to a CHS address, the LBA is divided into the correct cylinder, head, and sector values based on the geometry of the disk (sectors per track, number of heads, and number of cylinders). For example:
            - **Sector**: The LBA modulo the number of sectors per track + 1.
            - **Head**: The result of integer division of the LBA by sectors per track, modulo the number of heads.
            - **Cylinder**: The result of integer division of the LBA by sectors per track and heads.
    - **How Reading Works**: 
        - The disk spins, and the read/write head moves to the correct cylinder and head. The correct sector is read once the disk has aligned the sector under the head. This mechanical process makes reading sequential sectors more efficient than random access.

### 2. **FAT12 Headers in the Boot Sector and Their Importance**
- **FAT12 Headers in the Boot Sector**:
    - The boot sector in a FAT12 file system contains important metadata, called the BIOS Parameter Block (BPB) and Extended BIOS Parameter Block (EBPB). These headers provide crucial information about the layout and characteristics of the disk.
    - **Key Elements**:
        - `BPB_BYTES_PER_SECTOR`: Defines the size of each sector (typically 512 bytes).
        - `BPB_SECTORS_PER_CLUSTER`: The number of sectors per allocation unit (cluster).
        - `BPB_RESERVED_SECTORS`: The number of sectors reserved at the start of the volume, typically for the boot record.
        - `BPB_FILE_ALLOCATION_TABLES`: Specifies the number of FATs (usually 2).
        - `BPB_ROOT_DIR_ENTRIES`: Indicates how many entries can be stored in the root directory.
        - `BPB_TOTAL_SECTORS`: The total number of sectors in the disk.
        - **Extended Fields** like `EBPB_VOLUME_LABEL` and `EBPB_SYSTEM_ID` store additional metadata about the volume.
    - **Why Important**: 
        - These headers are essential for interpreting the disk structure and properly navigating the FAT12 file system. Without them, the bootloader wouldn’t know where to find key areas like the FAT tables, root directory, or data region.

### 3. **FAT12 Memory Layout: Reserved, FAT, Root, Data**
- **Reserved Area**:
    - This region typically contains the boot sector and other reserved sectors. In most cases, it starts at sector 1 and is followed by a few additional sectors reserved for future use.
    - **Boot Sector**: The first sector, which contains the BPB, EBPB, and the bootloader code that initializes the OS loading process.
    
- **FAT Region**:
    - The FAT (File Allocation Table) starts after the reserved sectors. FAT12 uses a 12-bit entry system to keep track of cluster chains.
    - **How It Works**: 
        - Each file is assigned clusters, and the FAT entries point to the next cluster in the file. A value of `0xFFF` signifies the end of the file.
        - There are usually two copies of the FAT stored for redundancy.

- **Root Directory**:
    - The root directory follows the FAT region and contains entries for files and directories. Each entry is 32 bytes, storing information such as the file name, size, and starting cluster number.
    - **Fixed Size**: FAT12 limits the number of root directory entries, typically to 224 (for floppy disks).

- **Data Area**:
    - This is where the actual file content is stored, and it begins right after the root directory. The clusters in this area hold the contents of the files and directories, with their order being managed by the FAT.
    - **Clustering**: Files are split into clusters, and each cluster's location is stored in the FAT. The first cluster of a file is stored in its directory entry.

### 4. **Finding the Kernel Code: From Bootloader to Kernel**
- **Locating the Kernel**:
    - The kernel is typically located in the data region, starting at a specific file or cluster. The bootloader needs to:
        1. **Read the FAT**: The bootloader uses the FAT to navigate the cluster chain for the kernel file.
        2. **Root Directory Lookup**: The bootloader looks up the root directory to find the starting cluster of the kernel.
        3. **Follow the Cluster Chain**: Using the FAT, the bootloader follows the cluster chain to load the entire kernel into memory.
        
- **Loading the Kernel**:
    - After identifying the starting cluster, the bootloader reads the kernel file's data from the disk, following each cluster until the end of the file is reached (signified by `0xFFF` in FAT12).
    - Once all the kernel data is loaded into memory (e.g., at address `0x9C00`), the bootloader transfers control to the kernel, allowing it to take over system execution.

### Conclusion
The addition of FAT12 support to the bootloader enables the TSOS project to efficiently load the kernel from a file system, opening the way for more complex and organized system structures. By understanding LBA, FAT12 headers, and memory layout, we ensure that the bootloader can accurately locate, load, and execute the kernel, forming a critical part of the system's boot process.