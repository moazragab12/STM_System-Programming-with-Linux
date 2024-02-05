#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


void
tokenizeInput (char *input, char ***tokens, int *numArgs)
{
  *numArgs = 0;			// Initialize the number of arguments

  char *token = strtok (input, " ");
  *tokens = NULL;		// Initialize the array of pointers to NULL

  while (token != NULL)
    {
      *tokens = realloc (*tokens, (*numArgs + 1) * sizeof (char *));
      if (*tokens == NULL)
	{
	  perror ("realloc");
	  exit (EXIT_FAILURE);
	}

      (*tokens)[(*numArgs)++] = strdup (token);	// Allocate memory for each token

      token = strtok (NULL, " ");
    }

  // Set the last argument to NULL to indicate the end
  *tokens = realloc (*tokens, (*numArgs + 1) * sizeof (char *));
  if (*tokens == NULL)
    {
      perror ("realloc");
      exit (EXIT_FAILURE);
    }
  (*tokens)[*numArgs] = NULL;
}

int
main (int argc, char *current_prog_argv[])
{
  int num_write = -1;
  unsigned char buf[300];

  char *cmd = NULL;
  char **tokens;
  int numArgs;
  size_t n = 0;
  while (1)
    {
      printf ("Hello brother lets start >> ");
      int size = getline (&cmd, &n, stdin);
      cmd[size - 1] = 0;
      //handling empty input
      if (strlen (cmd) == 0)
	continue;
      tokenizeInput (cmd, &tokens, &numArgs);	//tokenize the input


      if (strcmp ("exit", tokens[0]) == 0)
	{
	  printf ("GOOD BYE !\n ");
	  break;
	}
      else if (strcmp ("echo", tokens[0]) == 0)
	{

	  for (int i = 1; i < numArgs; i++)
	    {

	      //1 refers to stdout
	      int num_write_echo = write (1, tokens[i], strlen (tokens[i]));
	      write (1, " ", 1);

	      if (num_write_echo < 0)
		{

		  // Handle error if write fails

		  printf ("Couldnot write :Error");
		}
	    }

	  printf ("\n");

	}
      else if (strcmp ("pwd", tokens[0]) == 0)
	{

	  if (numArgs != 1)
	    {
	      printf ("NO_ OF ARGUMINTS is wrong:%d\n", numArgs);
	      printf ("Usage: %s\n", tokens[0]);

	    }
	  if (getcwd (buf, sizeof (buf)) == NULL)
	    {
	      printf ("The size of path is too way long");

	    }

	  num_write = write (1, buf, strlen (buf));
	  if (num_write < 0)
	    {
	      printf (">> Could not write to stdout\n");

	    }
	  printf ("\n");

	}
      else if (strcmp ("cd", tokens[0]) == 0)
	{

	  if (numArgs != 2)
	    {
	      printf ("Usage: %s Destination directory\n", tokens[0]);

	    }
	  if (chdir (tokens[1]) != 0)
	    {
	      printf ("There is an error in your cd\n");

	    }

	}
      else
	{
	  printf ("External command will be done\n");

	  pid_t returned_pid = fork ();
	  if (returned_pid > 0)
	    {
	      int wstatus;
	      wait (&wstatus);
	    }
	  else if (returned_pid == 0)
	    {
	      execvp (tokens[0], tokens);
	      printf ("I am not in the mode of execution. Exec failed\n");
	      return -1;
	    }
	  else
	    {
	      printf ("ERROR: I could not get a child\n");
	    }


	}
      free (cmd);
      //  free the allocated memory
      for (int i = 0; i < numArgs; ++i)
	{
	  free (tokens[i]);
	}
      free (tokens);		// Free the array of pointers
      cmd = NULL;
      n = 0;
    }
  return 0;
}
