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


