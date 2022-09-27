#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "shell.h"
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
      if (strncmp (command, builtInCommands[i],
                   strlen (builtInCommands[i])) == 0)
        return 1;
    }
  return 0;
}

int
isExecutable (char * command)
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
runCmd (int *pipe, char *command)
{
  int pid = fork ();
  if (pid == -1)
    {
      close (pipe[0]);
      close (pipe[1]);
      return -1;
    }
  if (pid == 0)
    {
      command [strlen (command) - 1] = '\0';
      printf ("%s\n", command);
      close (pipe[0]);
      dup2 (pipe[1], STDOUT_FILENO);
      execlp (command, command);
    }
  exit (0);
  return 0;
}