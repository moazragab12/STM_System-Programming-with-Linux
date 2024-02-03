#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define RET_STATUS 0
int main(int argc, char *argv[])
{
    const char *src = argv[1];
    const char *destination = argv[2];

    if (argc != 3) {
        printf("Usage: %s file_soure  file_destination\n", argv[0]);
        return -1;
    }

    if (rename(src, destination) != 0) {
        printf("There is an error with your mv\n");
        return -1;
    }
    printf("File moved Successfully from %s to %s \n", src, destination);
    return RET_STATUS;
}

