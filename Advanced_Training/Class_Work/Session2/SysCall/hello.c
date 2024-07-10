#include <stdio.h>
#include <string.h>
#include <unistd.h>


void print_wrapper(char * str)
{
	//printf("[Client] %s\n", str);
	//write(1, str, strlen(str));
	syscall(1, 1, str, strlen(str));
}

int main()
{
	print_wrapper("Hello World\n");

	return 0;
}
