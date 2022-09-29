#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "hash.h"
#include "process.h"

// No command line can be more than 100 characters
#define MAXLENGTH 100

char *PATH;

void
shell (FILE *input)
{

  PATH = calloc (1000, sizeof (PATH));
  char test[1000];
  getcwd (test, sizeof (test));
  strcpy (PATH, test);
  hash_init (100);
  hash_insert ("?", "0");
  char buffer[MAXLENGTH];
  while (1)
    {
      // Print the cursor and get the next command entered
      printf ("$ ");
      memset (buffer, 0, sizeof (buffer));
      if (fgets (buffer, MAXLENGTH, input) == NULL)
        {
          break;
        } 
      if (input != stdin)
        printf ("%s", buffer);
      if (strncmp (buffer, "echo", 4) == 0)
        echo (&buffer[5]);
      if (strncmp (buffer, "cd", 2) == 0)
        cd (&buffer[3]);
      if (strncmp (buffer, "pwd", 3) == 0)
        pwd ();
      if (strncmp (buffer, "which", 5) == 0)
        which (&buffer[6]);
      if (strncmp (buffer, "./bin/ls", 8) == 0)
        {
          char *arguments = &buffer[8];
          runCmd ("./bin/ls", arguments);
        }
      if (strncmp (buffer, "./bin/head", 10) == 0)
        {
          char *arguments = &buffer[10];
          runCmd ("./bin/head", arguments);
        }
      if (strncmp (buffer, "export", 6) == 0)
        {
          char *arguments = &buffer[7];
          export (arguments); 
        }
      if (strncmp (buffer, "quit", 4) == 0)
        break; 
    }
  printf ("\n");
  hash_destroy ();
}
