# Task_3 README
## Contents
### 1.mypwd.c
 - Description: This program `my_pwd.c` prints the current working directory 
using the ` getcwd `  and fixed buffer size
 - ####  command usage :
```
./mypwd
```
- #### Source code:
``` c
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
```
### 2.mymv.c
 - Description:  This program ` my_mv.c ` moves a file or directory from a source to a destination
   with Possibility to change it's name.

 - ####  command usage :
```
./mymv soruce_path Destination path
```
- #### Source code:
``` c
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
```
### 3.myecho.c
 - Description: This program `my_echo.c` prints the command line arguments as they are written 
 - ####  command usage :
```
./myecho arg1 arg2 ... argn
```
- #### Source code:
``` c
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

	// '1' in write function below refers to stdout
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
```
### 4.femtoshell.c
 - Description: This program `femtoshell.c` works as shell but with limited commands -you can guess it from
     it's name - ^_^   it provides `cd,pwd,echo `command and you can use `exit ` to terminate it 
 - ####  command usage :
```
./femtoshell
```
- #### Source code:
  
``` c
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
 ```
### Output examples:
- 1-mypwd
  
  ![image](https://github.com/moazragab12/STM_System-Programming-with-Linux/assets/123643446/8742eb15-6c07-4cfd-9dad-f450fbf4aa2f)
- 2-mymv
![image](https://github.com/moazragab12/STM_System-Programming-with-Linux/assets/123643446/8cfd2f5e-eea5-4d1a-9512-5f87b34958be)
  
- 3-myecho
![image](https://github.com/moazragab12/STM_System-Programming-with-Linux/assets/123643446/5aeaee0d-eba4-472e-8a47-b00f442c2c3e)

- 4-femtoshell  
![image](https://github.com/moazragab12/STM_System-Programming-with-Linux/assets/123643446/85b8f84c-1491-4d4e-b7b8-2f6418b36875)

### Note: make sure that executable permissions for each program before running.
### Building processing instruction :
- compile the source code using the `gcc` compiler and use this command:
  ```
  gcc -o filename_asyouwish file.c --save-temps
  ```
 ### example:
```
  gcc -o femtoshell femtoshell.c --save-temps
  ```
