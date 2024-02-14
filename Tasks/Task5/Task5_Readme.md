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

### code
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#define SECTOR_SIZE 512
#define MAX_LOGICAL_PARTITIONS 100	// Maximum expected logical partitions

typedef struct {
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partition_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
} PartitionEntry;

void readPartitionTable(int fd, PartitionEntry * partitionTable)
{
    char buf[SECTOR_SIZE];
    ssize_t bytes_read = read(fd, buf, SECTOR_SIZE);
    if (bytes_read == -1) {
	perror("read");
	exit(EXIT_FAILURE);
    }

    PartitionEntry *table_entry_ptr = (PartitionEntry *) & buf[446];
    for (int i = 0; i < 4; i++) {
	partitionTable[i] = table_entry_ptr[i];
    }
}

void printLogicalPartitions(int fd, uint32_t extended_lba,
			    const char *device)
{
    lseek(fd, extended_lba * SECTOR_SIZE, SEEK_SET);
    PartitionEntry logical_partition;
    int logical_partition_number = 5;	// First logical partition number
    while (read(fd, &logical_partition, sizeof(PartitionEntry)) > 0) {
	if (logical_partition.partition_type != 0) {
	    printf("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X\n",
		   device,
		   logical_partition_number,
		   ' ',
		   logical_partition.lba,
		   logical_partition.lba + logical_partition.sector_count -
		   1, logical_partition.sector_count,
		   (uint32_t) (((uint64_t) logical_partition.sector_count *
				512) / (1024 * 1024 * 1024)),
		   logical_partition.partition_type);
	    logical_partition_number++;
	}
    }
}

void printPartitionInfo(int fd, PartitionEntry * partitionTable,
			const char *device)
{
    printf("%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Device",
	   "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");
    int logicalPartitionCount = 0;
    for (int i = 0; i < 4; i++) {
	if (partitionTable[i].partition_type == 0x05
	    || partitionTable[i].partition_type == 0x0F) {
	    // Logical partition, read extended partition table
	    uint32_t extended_lba = partitionTable[i].lba;
	    printLogicalPartitions(fd, extended_lba, device);
	    logicalPartitionCount++;
	} else if (partitionTable[i].partition_type != 0) {
	    printf("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X\n",
		   device,
		   i + 1 - logicalPartitionCount,
		   partitionTable[i].status == 0x80 ? '*' : ' ',
		   partitionTable[i].lba,
		   partitionTable[i].lba + partitionTable[i].sector_count -
		   1, partitionTable[i].sector_count,
		   (uint32_t) (((uint64_t) partitionTable[i].sector_count *
				512) / (1024 * 1024 * 1024)),
		   partitionTable[i].partition_type);
	}
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
	printf("Usage: %s <device>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
	perror("open");
	exit(EXIT_FAILURE);
    }

    PartitionEntry partitionTable[4];
    readPartitionTable(fd, partitionTable);
    printPartitionInfo(fd, partitionTable, argv[1]);

    if (close(fd) == -1) {
	perror("close");
	exit(EXIT_FAILURE);
    }

    return 0;
}
```

### Features
- Reads and prints primary partitions from the MBR partition table.
- Reads and prints logical partitions within extended partitions.
- Handles various partition types and displays partition information accordingly.
- Provides error handling for file operations and system calls.

###  Requirements
- Linux-based operating system.
- C compiler (e.g., GCC).
