# TASK_2 README

## Description
This C programme prompts the user to input text until they choose to terminate the program. It utilizes the `fgets` function for input, `strcspn` for removing the newline character at the end to facilitate comparison with the string "exit." Upon requesting program termination, the message "Good Bye !" is displayed before concluding the execution.

## C Code (task_2.c)
```c

       #include<stdio.h>
        #include<string.h>
      int main(void)
      {
              while(1)
              {       printf("Hello brother lets start > ");
      char input[200];
      fgets(input,200,stdin);
      input[strcspn(input,"\n")]='\0';
      
      if(strcmp("exit",input)==0)
      {printf("GOOD BYE !\n ");
              break;
      }
      else
      printf("you said :%s\n ",input);
                }
              return 0;
              }
        
                                                          
```
## Compilation process commands
#### 1.Preprocess:
```
gcc task_2.c -E -o task_2.i
```
* ```-o ``` is used in redirecting the output file to task_2.i 

- ```-E``` is used to stop the compiler after preprocessing stage
#### 2.Compile Assembly Code:
```
gcc task_2.c -S -o task_2.s
```
* ```-S``` is used to stop the compiler after generating assembly file
#### 3.Compile Object File:
```
gcc task_2.c -c -o task_2.o
```
* ```-c```compiles the source code into an object file before linking
#### 4.Final Compilation (After linking):
```
gcc task_2.c -o task_2
```
* This command links the object file and generates the executable file
## Execution of programme 
running the compiled programme while you are in the same directory
``` ./task_2 ```
### output example

