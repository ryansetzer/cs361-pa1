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
      char bufferTwin[MAXLENGTH];
      memcpy (bufferTwin, buffer, MAXLENGTH);
      char *argumentOne = strtok (bufferTwin, "|");
      char *argumentTwo = strtok (NULL, "|");
      if (argumentTwo != NULL)
        {
          argumentTwo [strlen (argumentTwo) - 1] = '\0';
          //printf ("FROM SHELL : command one: [%s], command two: [%s]\n", argumentOne, argumentTwo);
          runExec (argumentOne, &argumentTwo[1]);
        }
      else
        {
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
              int fd[2];
              pipe (fd);
              char *arguments = &buffer[8];
              runCmd ("./bin/ls", arguments, fd);
              char tempBuf[1000];
              for (int i = 0; i < sizeof (tempBuf); i++)
                tempBuf[i] = '\0';
              read (fd[0], tempBuf, sizeof (tempBuf));
              if (tempBuf[0] != ' ')
                printf ("%s", tempBuf);
            }
          if (strncmp (buffer, "./bin/head", 10) == 0)
            {
              int fd[2];
              pipe (fd);
              char *arguments = &buffer[10];
              runCmd ("./bin/head", arguments, fd);
              char tempBuf[1000];
              for (int i = 0; i < sizeof (tempBuf); i++)
                tempBuf[i] = '\0';
              read (fd[0], tempBuf, sizeof (tempBuf));
              if (tempBuf[0] != ' ')
                printf ("%s", tempBuf);
            }
          if (strncmp (buffer, "export", 6) == 0)
            export(&buffer[7]);
          if (strncmp (buffer, "quit", 4) == 0)
            break;
        }
    }
  printf ("\n");
  hash_destroy ();
}
