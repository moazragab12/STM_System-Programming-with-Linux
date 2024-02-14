## MBR Partition Table Reader

This program reads the Master Boot Record (MBR) partition table of a disk device and prints the partition information, including primary partitions and logical partitions if they exist.

### Usage

#### Compilation

Compile the program using a C compiler:

```bash
gcc -o myfdisk myfdisk.c
```

### Execution
Run the program with the disk device as an argument:
```
./myfdisk /dev/sdx
```
Replace '/dev/sdx' with the appropriate disk device name.

### Features
- Reads and prints primary partitions from the MBR partition table.
- Reads and prints logical partitions within extended partitions.
- Handles various partition types and displays partition information accordingly.
- Provides error handling for file operations and system calls.

###  Requirements
- Linux-based operating system.
- C compiler (e.g., GCC).
