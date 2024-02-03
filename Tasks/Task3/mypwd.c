#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>

#define RET_STATUS 0
int main(int argc, char *argv[])
{
    int num_write;
    unsigned char buf[400];

    if (argc != 1) {
	printf("Usage: %s\n", argv[0]);
	return -1;
    }
    if (getcwd(buf, sizeof(buf)) == NULL) {
	printf("The size of path is too way long");
	return -1;
    }

    num_write = write(1, buf, strlen(buf));
    if (num_write < 0) {
	printf(">> Could not write to stdout\n");
	return -1;
    }
    printf("\n");
    return RET_STATUS;
}
