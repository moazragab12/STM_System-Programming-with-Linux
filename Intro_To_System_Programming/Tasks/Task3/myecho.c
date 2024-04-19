#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define RET_STATUS 0

int main(int argc, char *argv[])
{

    for (int i = 1; i < argc; i++) {

	//1 refers to stdout
	int num_write = write(1, argv[i], strlen(argv[i]));
	write(1, " ", 1);

	if (num_write < 0) {

	    // Handle error if write fails

	    printf("Couldnot write :Error");

	    return -1;

	}
    }

    printf("\n");

    return RET_STATUS;

}
