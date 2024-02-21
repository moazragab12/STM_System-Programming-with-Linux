#define _LARGEFILE64_SOURCE

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
    char buf[512];
    lseek64(fd, (off64_t) extended_lba * SECTOR_SIZE, SEEK_SET);
    int logical_partition_number = 5;	// First logical partition number
    read(fd, buf, SECTOR_SIZE);
    PartitionEntry *logical_partition = (PartitionEntry *) & buf[446];
    while (1) {
	off64_t offsetofpartition =
	    extended_lba + logical_partition[1].lba;
	lseek64(fd, (off64_t) offsetofpartition * SECTOR_SIZE, SEEK_SET);
	read(fd, buf, SECTOR_SIZE);
	logical_partition = (PartitionEntry *) & buf[446];
	{
	    printf("%-8s%-4d  %-4c %-10lu %-10lu %-10u %06.2fG %5X\n",
		   device,
		   logical_partition_number,
		   ' ',
		   logical_partition->lba + offsetofpartition,
		   logical_partition->lba +
		   logical_partition->sector_count - 1 + offsetofpartition,
		   logical_partition->sector_count,
		   (double) ((uint64_t) logical_partition->sector_count *
			     SECTOR_SIZE) / (1024 * 1024 * 1024),
		   logical_partition->partition_type);
	    logical_partition_number++;
	}
	if (logical_partition[1].lba == 0)
	    break;

    }
}

void printPartitionInfo(int fd, PartitionEntry * partitionTable,
			const char *device)
{
    printf("%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Device",
	   "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");
    int ext_flag = 5;
    for (int i = 0; i < 4; i++) {
	if (partitionTable[i].partition_type == 5) {
	    ext_flag = i;
	}
	if (partitionTable[i].sector_count != 0) {
	    printf("%-8s%-4d  %-4c %-10u %-10u %-10u %06.2fG %5X\n",
		   device,
		   i + 1,
		   partitionTable[i].status == 0x80 ? '*' : ' ',
		   partitionTable[i].lba,
		   partitionTable[i].lba + partitionTable[i].sector_count -
		   1, partitionTable[i].sector_count,
		   (double) ((uint64_t) partitionTable[i].sector_count *
			     SECTOR_SIZE) / (1024 * 1024 * 1024),
		   partitionTable[i].partition_type);
	}
    }
    if (ext_flag < 5) {
	// Logical partition, read extended partition table
	uint32_t extended_lba = partitionTable[ext_flag].lba;
	printLogicalPartitions(fd, extended_lba, device);

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
