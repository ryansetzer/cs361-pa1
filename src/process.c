#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
  char *builtInCommands[] = { "cd", "echo", "pwd", "which" };
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
      char *token2 = strtok (NULL, "\0");
      printf ("%s\n", writeableArgument);
      close (fd[0]); // close read end of pipe
      dup2 (fd[1], STDOUT_FILENO);
      if (strlen (writeableArgument) == 0)
        execlp (command, command, NULL);
      else
        if (token2 == NULL || token1 == NULL)
          execlp (command, command, writeableArgument, NULL);
        else
          execlp (command, command, token1, token2, NULL);
    }
  char buffer[1000];
  for (int i = 0; i < sizeof (buffer); i++)
    buffer[i] = 0;
  read (fd[0], buffer, sizeof (buffer));
  if (strncmp (" ", buffer, sizeof (buffer)) != 0)
    printf ("%s", buffer);
  return 0;
}
