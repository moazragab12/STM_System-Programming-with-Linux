#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void tokenizeInput(char *input, char ***tokens, int *numArgs)
{
    *numArgs = 0;
    char *token = strtok(input, " ");
    *tokens = NULL;

    while (token != NULL) {
	*tokens = realloc(*tokens, (*numArgs + 1) * sizeof(char *));
	if (*tokens == NULL) {
	    perror("realloc");
	    exit(EXIT_FAILURE);
	}

	(*tokens)[(*numArgs)++] = strdup(token);
	token = strtok(NULL, " ");
    }

    *tokens = realloc(*tokens, (*numArgs + 1) * sizeof(char *));
    if (*tokens == NULL) {
	perror("realloc");
	exit(EXIT_FAILURE);
    }
    (*tokens)[*numArgs] = NULL;
}

int main(int argc, char *current_prog_argv[])
{
    char *cmd = NULL;
    char **tokens;
    int numArgs;
    size_t n = 0;

    while (1) {
	printf("Hello brother lets start >> ");
	int size = getline(&cmd, &n, stdin);
	if (size == -1) {
	    perror("getline");
	    exit(EXIT_FAILURE);
	}
	cmd[size - 1] = 0;

	// Handling empty input
	if (strlen(cmd) == 0)
	    continue;

	tokenizeInput(cmd, &tokens, &numArgs);	// Tokenize the input

	// Check for redirections
	int input_redirection_index = -1;
	int output_redirection_index = -1;
	for (int i = 0; i < numArgs; i++) {
	    if (strcmp("<", tokens[i]) == 0) {
		input_redirection_index = i;
	    } else if (strcmp(">", tokens[i]) == 0) {
		output_redirection_index = i;
	    }
	}

	if (strcmp("exit", tokens[0]) == 0) {
	    if (input_redirection_index != -1
		|| output_redirection_index != -1)
		fprintf(stderr,
			"Cannot use input/output redirection with exit\n");
	    else if (numArgs != 1)
		printf("wrong no of args \n ");
	    else {
		printf("GOOD BYE !\n ");
		break;
	    }
	} else if (strcmp("echo", tokens[0]) == 0) {
	    // Handle input redirection
	    if (input_redirection_index != -1)
		fprintf(stderr,
			"Cannot use input redirection with echo\n");
	    else {
		// Handle output redirection
		if (output_redirection_index != -1) {
		    int fd_out = open(tokens[output_redirection_index + 1],
				      O_CREAT | O_WRONLY | O_TRUNC, 0644);
		    if (fd_out < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		    }



		    for (int i = 1; i < output_redirection_index; i++) {
			if (i > 1) {
			    // Separate each token by a space
			    dprintf(fd_out, " ");
			}
			// Write the token to the file
			dprintf(fd_out, "%s", tokens[i]);
		    }
		    // Write a newline character to the file
		    dprintf(fd_out, "%s", "\n");
		    close(fd_out);
		}
		// Echo command implementation

		else {
		    for (int i = 1; i < numArgs; i++) {
			printf("%s ", tokens[i]);
		    }
		    printf("\n");
		}
	    }
	} else if (strcmp("pwd", tokens[0]) == 0) {
	    char buf[300];
	    if (output_redirection_index == -1 && numArgs > 1)
		printf("Wrong no of args to pwd\n");
	    // Handle input redirection
	    else if (input_redirection_index != -1)
		fprintf(stderr, "Cannot use input redirection with pwd\n");
	    else {
		// Handle output redirection
		if (output_redirection_index != -1) {
		    int fd_out = open(tokens[output_redirection_index + 1],
				      O_CREAT | O_WRONLY | O_TRUNC, 0644);
		    if (fd_out < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		    }
// Append a newline character to the end of the string
		    if (getcwd(buf, sizeof(buf)) == NULL) {
			printf("The size of path is too way long\n");
			continue;
		    }

		    size_t len = strlen(buf);
		    buf[len] = '\n';
		    buf[len + 1] = '\0';	// Null-terminate the string
		    dprintf(fd_out, "%s", buf);

		    close(fd_out);
		}
		// Pwd command implementation

		else {
		    if (getcwd(buf, sizeof(buf)) == NULL) {
			printf("The size of path is too way long\n");
			continue;
		    }
		    printf("%s\n", buf);

		}
	    }
	} else if (strcmp("cd", tokens[0]) == 0) {
	    // Handle input/output redirection
	    if (input_redirection_index != -1
		|| output_redirection_index != -1)
		fprintf(stderr,
			"Cannot use input/output redirection with cd\n");
	    else if (numArgs != 2)
		printf("wrong no of args \n");
	    else {
		// Handle cd command
		if (numArgs != 2) {
		    printf("Usage: %s Destination directory\n", tokens[0]);
		} else {
		    if (chdir(tokens[1]) != 0) {
			printf("There is an error in your cd\n");
		    }
		}
	    }
	} else {
	    // External command execution
	    printf("External command will be executed\n");
	    pid_t returned_pid = fork();
	    if (returned_pid > 0) {
		// Parent process waits for the child to complete
		int wstatus;
		wait(&wstatus);
	    } else if (returned_pid == 0) {
		// Child process executes the command
		// Handle input redirection
		if (input_redirection_index != -1) {
		    int fd_in = open(tokens[input_redirection_index + 1],
				     O_RDONLY);
		    if (fd_in < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		    }
		    dup2(fd_in, STDIN_FILENO);
		    close(fd_in);

		    // Remove "<" and the filename from the tokens array
		    tokens[input_redirection_index] = NULL;
		    tokens[input_redirection_index + 1] = NULL;
		}
		// Handle output redirection
		if (output_redirection_index != -1) {
		    int fd_out = open(tokens[output_redirection_index + 1],
				      O_CREAT | O_WRONLY | O_TRUNC, 0644);
		    if (fd_out < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		    }
		    dup2(fd_out, STDOUT_FILENO);
		    close(fd_out);

		    // Remove ">" and the filename from the tokens array
		    tokens[output_redirection_index] = NULL;
		    tokens[output_redirection_index + 1] = NULL;
		}
		// Execute the command
		execvp(tokens[0], tokens);
		// If execvp returns, it means an error occurred
		fprintf(stderr, "Command not found: %s\n", tokens[0]);
		exit(EXIT_FAILURE);
	    } else {
		fprintf(stderr, "ERROR: Failed to fork a child process\n");
	    }
	}

	free(cmd);		// Free the allocated memory
	// Free the array of pointers
	for (int i = 0; i < numArgs; ++i) {
	    free(tokens[i]);
	}
	free(tokens);
	cmd = NULL;
	n = 0;
    }

    return 0;
}
