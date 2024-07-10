#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//int main(int argc, char** argv)
int main(int argc, char *current_prog_argv[])
{
    printf("Num of args = %d\n", argc);

    for (int i = 0; i < argc; i++) {
	printf("argv[%d] = %s\n", i, current_prog_argv[i]);
    }

    getchar();

    char *cmd = NULL;
    size_t n = 0;
    while (1) {
	printf(">>>>>>> ");
	int size = getline(&cmd, &n, stdin);
	cmd[size - 1] = 0;
	if (strlen(cmd) == 0)
	    continue;

	int fd = open("test.txt", O_WRONLY);
	if (fd < 0) {
	    printf(">> Could not open the file\n");
	    return -1;
	}
	printf(">> the file opend with fd = %d\n", fd);

	off_t newoff = lseek(fd, 100000, SEEK_END);
	write(fd, "Hello\n", 7);


	pid_t returned_pid = fork();
	if (returned_pid > 0) {
	    int wstatus;
	    printf
		("PARENT: My PID = %d,  I have a new child and his PID = %d\n",
		 getpid(), returned_pid);
	    off_t offset_from_parent = lseek(fd, 0, SEEK_CUR);

	    printf("PARENT: offset_from_parent = %ld, newofffest = %ld\n",
		   offset_from_parent, newoff);


	    wait(&wstatus);
	    offset_from_parent = lseek(fd, 0, SEEK_CUR);
	    printf
		("PARENT: after wait: offset_from_parent = %ld, newofffest = %ld\n",
		 offset_from_parent, newoff);
	} else if (returned_pid == 0) {
	    printf("CHILD: My PID = %d,  My parent PID = %d\n", getpid(),
		   getppid());
	    getchar();
	    off_t offset_from_child = lseek(fd, 0, SEEK_CUR);

	    printf("CHILD: offset_from_child = %ld, newofffest = %ld\n",
		   offset_from_child, newoff);

	    char *new_program_argv[] = { NULL };
	    char *new_program_envp[] = { NULL };

	    off_t newoff = lseek(fd, 5000, SEEK_END);
	    offset_from_child = lseek(fd, 0, SEEK_CUR);
	    printf
		("CHILD: another seek: offset_from_child = %ld, newofffest = %ld\n",
		 offset_from_child, newoff);
	    //execve(cmd, new_program_argv, new_program_envp);

	    printf("I am not in the mode of execution. Exec failed\n");

	    return -1;
	} else {
	    printf("ERROR: I could not get a child\n");
	}

	free(cmd);
	cmd = NULL;
	n = 0;
    }
    return 0;
}
