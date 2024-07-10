#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define RET_STATUS 0



//int main(int argc, char** argv)
int main(int argc, char* argv[])
{

    int fd, num_write;
    unsigned char buf[10] = {66, 67, 68, 69, 66, 67, 68, 69, 66, 67};

    fd = open(argv[1], O_WRONLY);
    if (fd < 0) {
	printf(">> Could not open the file\n");
	return -1;
    }
    printf(">> the file opend with fd = %d\n", fd);

    off_t newoff = lseek(fd, 100000, SEEK_END);
    printf(">> the file new offset = %lu\n", newoff);
    num_write = write(fd, buf, 10);
    printf(">> the file is written with %u bytes\n", num_write);

    close(fd);

    return RET_STATUS;
}
