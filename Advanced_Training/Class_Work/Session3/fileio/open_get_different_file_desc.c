#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define RET_STATUS 0



//int main(int argc, char** argv)
int main(int argc, char* argv[])
{

    int fd, num_write, fd2;
    unsigned char buf[10] = {66, 67, 68, 69, 66, 67, 68, 69, 66, 67};

    fd = open(argv[1], O_WRONLY);
    if (fd < 0) {
	printf(">> Could not open the file\n");
	return -1;
    }
    printf(">> the file opend with fd = %d\n", fd);

    fd2 = open(argv[1], O_WRONLY);
    if (fd2 < 0) {
	printf(">> Could not open the file\n");
	return -1;
    }
    printf(">> the file opend with fd2 = %d\n", fd2);

    off_t newoff = lseek(fd, 100000, SEEK_END);

    int new_fd = dup(fd);

    off_t offset_from_newfd = lseek(new_fd, 0, SEEK_CUR);

    printf("offset_from_newfd = %ld, newofffest = %ld\n", offset_from_newfd, newoff);
    off_t offset_from_fd2 = lseek(fd2, 0, SEEK_CUR);
    printf("offset_from_fd2 = %ld, newofffest = %ld\n", offset_from_fd2, newoff);

    printf(">> the file new offset = %lu\n", newoff);
    num_write = write(fd, buf, 10);
    printf(">> the file is written with %u bytes\n", num_write);

    close(fd);

    return RET_STATUS;
}
