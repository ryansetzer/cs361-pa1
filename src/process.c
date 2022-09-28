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
runCmd (int *pipe, char *command, char *arguments)
{
  printf ("Working here\n");
  int pid = fork ();
  if (pid == -1)
    {
      printf ("Inproper child fork\n");
      close (pipe[0]);
      close (pipe[1]);
      return -1;
    }
  if (pid == 0)
    {
      printf ("Proper child fork\n");
      char *writeableArgument = strdup (&arguments[1]);
      writeableArgument[strlen (writeableArgument) - 1] = '\0';
      printf ("command: %s, arguments: %s\n", command, writeableArgument);
      close (pipe[0]); // close read end of pipe
      printf ("Here\n");
      dup2 (pipe[1], STDOUT_FILENO);
      if (strncmp ("", writeableArgument, strlen (writeableArgument)) == 0)
        execlp (command, command, NULL);
      else
        execlp (command, command, writeableArgument, NULL);
    }
  exit (0);
  return 0;
}
