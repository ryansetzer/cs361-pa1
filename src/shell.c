#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "hash.h"
#include "process.h"

// No command line can be more than 100 characters
#define MAXLENGTH 100

void
shell (FILE *input)
{
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
      else if (strncmp (buffer, "cd", 2) == 0)
        cd (&buffer[3]);
      else if (strncmp (buffer, "pwd", 3) == 0)
        pwd ();
      else if (strncmp (buffer, "which", 5) == 0)
        which (&buffer[6]);
      else if (strncmp (buffer, "./bin/", 6) == 0)
        runCmd (buffer);
      else if (strncmp (buffer, "export", 6) == 0)
        export(&buffer[7]);
      else if (strncmp (buffer, "quit", 4) == 0)
        break;
        // }
    }
  printf ("\n");
  hash_destroy ();
}
