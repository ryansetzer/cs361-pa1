#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"

// The contents of this file are up to you, but they should be related to
// running separate processes. It is recommended that you have functions
// for:
//   - performing a $PATH lookup
//   - determining if a command is a built-in or executable
//   - running a single command in a second process
//   - running a pair of commands that are connected with a pipe

int
isBuiltIn (char *command)
{
  char *builtInCommands[] = { "cd", "echo", "pwd", "which", "export" };
  for (int i = 0; i < sizeof (builtInCommands) / sizeof (builtInCommands[0]);
       i++)
    {
      if (strncmp (command, builtInCommands[i], strlen (builtInCommands[i]))
          == 0)
        return 1;
    }
  return 0;
}

int
isExecutable (char *command)
{
  char *executables[] = { "ls", "cat", "env", "head", "rm" };
  for (int i = 0; i < sizeof (executables) / sizeof (executables[0]); i++)
    {
      if (strncmp (command, executables[i], strlen (executables[i])) == 0)
        return 1;
    }
  return 0;
}

int
runCmd (char *command, char *arguments)
{
  int fd[2];
  pipe (fd);
  int pid = fork ();
  if (pid == -1)
    {
      close (fd[0]);
      close (fd[1]);
      return -1;
    }
  if (pid == 0)
    {
      char *writeableArgument = strdup (&arguments[1]);
      writeableArgument[strlen (writeableArgument) - 1] = '\0';
      char *token1 = strtok (writeableArgument, " ");
      char *token2 = strtok (NULL, " ");
      char *token3 = strtok (NULL, "\\n");
      // printf ("token1=%s   token2=%s   token3=%s\n", token1, token2, token3);
      // printf ("%s\n", writeableArgument);
      close (fd[0]); // close read end of pipe
      dup2 (fd[1], STDOUT_FILENO);
      if (strlen (writeableArgument) == 0)
        execlp (command, command, NULL);
      else if (token2 == NULL || token1 == NULL)
        execlp (command, command, writeableArgument, NULL);
      else if (token1 != NULL && token2 != NULL && token3 != NULL)
        execlp (command, command, token1, token2, token3, NULL);
      else
        execlp (command, command, token1, token2, NULL);
    }
  char buffer1[1000];
  read (fd[0], buffer1, sizeof (buffer1));
  if (strncmp (" ", buffer1, sizeof (buffer1)) != 0)
    printf ("%s", buffer1);
  return 0;
}

void
runExec (char *commandOne, char *commandTwo)
{
  int fd[2];
  pipe (fd);

  pid_t child = fork ();

  if (child == 0)
    {
      close (fd[0]); // close read end of pipe
      dup2 (fd[1], STDOUT_FILENO);
      if (strncmp (commandOne, "cd", 2) == 0)
        cd (&commandOne[3]);
      if (strncmp (commandOne, "pwd", 3) == 0)
        pwd ();
      if (strncmp (commandOne, "which", 5) == 0)
        which (&commandOne[6]);
      if (strncmp (commandOne, "./bin/ls", 8) == 0)
        {
          char *arguments = &commandOne[8];
          runCmd ("./bin/ls", arguments);
        }
      if (strncmp (commandOne, "./bin/head", 10) == 0)
        {
          char *arguments = &commandOne[10];
          runCmd ("./bin/head", arguments);
        }
      if (strncmp (commandOne, "export", 6) == 0)
        {
          char *arguments = &commandOne[7];
          export(arguments);
        }
    }
  else
    {
      close (fd[1]); // close write end of pipe
//      if (strncmp (commandTwo, "./bin/head", 10) == 0)
//        {
//          dup2 (fd[0], STDOUT_FILENO);
//          execlp ("./bin/head", "./bin/head", fd[0], NULL);
//        }
      char buffy[1000];
      read (fd[0], buffy, sizeof (buffy));
      printf ("[%s]\n", buffy);
    }   
}







