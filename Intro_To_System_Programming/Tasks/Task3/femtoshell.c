#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_ARGS_EXPECTED 200

int main(void)
{
    char input[300];
    int num_write = -1;
    unsigned char buf[400];


    while (1) {

	printf("Hello brother lets start > ");
	int argumints = 0;	//no of argumints

	char *tokens[300];

	fgets(input, 300, stdin);

	input[strcspn(input, "\n")] = '\0';

	char *token = strtok(input, " ");

	 while (token != NULL && argumints < MAX_ARGS_EXPECTED) {
	    tokens[argumints++] = token;
	    token = strtok(NULL, " ");

	}
// Set the last argument to NULL to indicate the end
	tokens[argumints] = NULL;

	if (strcmp("exit", tokens[0]) == 0) {
	    printf("GOOD BYE !\n ");
	    break;
	} else if (strcmp("echo", tokens[0]) == 0) {

	    for (int i = 1; i < argumints; i++) {

		//1 refers to stdout
		int num_write_echo =
		    write(1, tokens[i], strlen(tokens[i]));
		write(1, " ", 1);

		if (num_write_echo < 0) {

		    // Handle error if write fails

		    printf("Couldnot write :Error");


		}
	    }

	    printf("\n");

	} else if (strcmp("pwd", tokens[0]) == 0) {

	    if (argumints != 1) {
		printf("NO_ OF ARGUMINTS is wrong:%d\n",argumints);
    		    printf("Usage: %s\n", tokens[0]);
	
	    }
	    if (getcwd(buf, sizeof(buf)) == NULL) {
		printf("The size of path is too way long");
	
	    }

	    num_write = write(1, buf, strlen(buf));
	    if (num_write < 0) {
		printf(">> Could not write to stdout\n");
	
	    }
	    printf("\n");

	} else if (strcmp("cd", tokens[0]) == 0) {

	    if (argumints != 2) {
		printf("Usage: %s Destination directory\n", tokens[0]);

	    }
	    if (chdir(tokens[1]) != 0) {
		printf("There is an error in your cd\n");
		
	    }

	} else {
	    printf("error: unsupported command or misspelled command \n");
	}
    }
    return 0;

}
