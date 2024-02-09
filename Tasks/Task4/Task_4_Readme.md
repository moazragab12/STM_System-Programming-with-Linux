# Picoshell

This project is a command-line program that performs various operations based on user input.

## Description

The program allows the user to execute built-in commands like `echo`, `pwd`, and `exit`. It tokenizes the user input and performs the corresponding action based on the command.
and if he doesnot find this in my built-in commands it will go to external commands by its name and argumints
## Installation

To run this program, you need to have a C compiler installed on your system.

## Usage

1. Compile the code using the following command:
    ```
    gcc picoshell.c -o picoshell
    ```

2. Run the program using the following command:
    ```
    ./picoshell
    ```

3. Enter commands when prompted and press Enter to execute them.

## Example Commands

- `echo Hello World`: Prints "Hello World" to the console.
- `pwd`: Prints the current working directory.
- `exit`: Exits the program.

  ## c code
``` c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


/*
 * Function: tokenizeInput
 * -------------------------
 * Tokenizes the input string and stores the tokens in a dynamically allocated array.
 *
 * input: The input string to be tokenized.
 * tokens: Pointer to a pointer where the tokens will be stored.
 * numArgs: Pointer to an integer where the number of arguments will be stored.
 */
void tokenizeInput(char *input, char ***tokens, int *numArgs) {
    *numArgs = 0;			// Initialize the number of arguments
    char *token = strtok(input, " ");
    *tokens = NULL;			// Initialize the array of pointers to NULL

    // Tokenize the input string
    while (token != NULL) {
        // Allocate memory for the token array
        *tokens = realloc(*tokens, (*numArgs + 1) * sizeof(char *));
        if (*tokens == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        // Allocate memory for each token and copy it
        (*tokens)[(*numArgs)++] = strdup(token);

        // Get the next token
        token = strtok(NULL, " ");
    }

    // Add a NULL pointer at the end of the token array to indicate the end
    *tokens = realloc(*tokens, (*numArgs + 1) * sizeof(char *));
    if (*tokens == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    (*tokens)[*numArgs] = NULL;
}


/*
 * Function: main
 * --------------
 * The main function of the program. Implements a simple shell.
 */
int main(int argc, char *current_prog_argv[]) {
    int num_write = -1;
    unsigned char buf[300];
    char *cmd = NULL;
    char **tokens;
    int numArgs;
    size_t n = 0;

    // Main loop for the shell
    while (1) {
        printf("Hello brother lets start >> ");

        // Read input from the user
        int size = getline(&cmd, &n, stdin);
        cmd[size - 1] = 0;  // Remove newline character

        // Handling empty input
        if (strlen(cmd) == 0)
            continue;

        // Tokenize the input command
        tokenizeInput(cmd, &tokens, &numArgs);

        // Execute commands based on the first token
        if (strcmp("exit", tokens[0]) == 0) {
            // Exit the shell
            printf("GOOD BYE !\n ");
            break;
        } else if (strcmp("echo", tokens[0]) == 0) {
            // Echo command implementation
            for (int i = 1; i < numArgs; i++) {
                // Write each token to stdout
                int num_write_echo = write(1, tokens[i], strlen(tokens[i]));
                write(1, " ", 1);
                if (num_write_echo < 0) {
                    printf("Could not write: Error\n");
                }
            }
            printf("\n");
        } else if (strcmp("pwd", tokens[0]) == 0) {
            // Pwd command implementation
            if (numArgs != 1) {
                printf("Usage: %s\n", tokens[0]);
            }
            if (getcwd(buf, sizeof(buf)) == NULL) {
                printf("The size of path is too long\n");
            }
            num_write = write(1, buf, strlen(buf));
            if (num_write < 0) {
                printf(">> Could not write to stdout\n");
            }
            printf("\n");
        } else if (strcmp("cd", tokens[0]) == 0) {
            // Cd command implementation
            if (numArgs != 2) {
                printf("Usage: %s Destination directory\n", tokens[0]);
            }
            if (chdir(tokens[1]) != 0) {
                printf("There is an error in your cd\n");
            }
        } else {
            // External command execution
            printf("External command will be done\n");
            pid_t returned_pid = fork();
            if (returned_pid > 0) {
                // Parent process waits for the child to complete
                int wstatus;
                wait(&wstatus);
            } else if (returned_pid == 0) {
                // Child process executes the command
                execvp(tokens[0], tokens);
                printf("I am not in the mode of execution. Exec failed\n");
                return -1;
            } else {
                printf("ERROR: I could not get a child\n");
            }
        }

        // Free allocated memory
        free(cmd);
        for (int i = 0; i < numArgs; ++i) {
            free(tokens[i]);
        }
        free(tokens);
        cmd = NULL;
        n = 0;
    }
    return 0;
}


 ```
### Output examples:
![image](https://github.com/moazragab12/STM_System-Programming-with-Linux/assets/123643446/1091cca2-dc4e-4343-ab4e-ab2358ac2b55)

### Note: make sure that executable permissions for each program before running.

### Building processing instruction :
- compile the source code using the `gcc` compiler and use this command:
  ```
  gcc -o filename_asyouwish file.c --save-temps
  ```
 ### example:
```
  gcc -o picoshell picoshell.c --save-temps
  ```
## License

This project is licensed under the [MIT License](LICENSE).
