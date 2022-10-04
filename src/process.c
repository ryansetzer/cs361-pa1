#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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

void
runCmd (char *command, char *arguments, int fd[2])
{
  pid_t child_pid = fork();

  if (child_pid < 0)
    return;

  if (child_pid == 0)
    {
      char *writeableArgument = strdup (&arguments[1]);
      writeableArgument[strlen (writeableArgument) - 1] = '\0';
      char *token1 = strtok (writeableArgument, " ");
      char *token2 = strtok (NULL, " ");
      char *token3 = strtok (NULL, "\\n");
      //printf ("token1=[%s] token2=[%s] token3=[%s] command: [%s]\n", token1, token2, token3, command);
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
}

void
runExec (char *commandOne, char *commandTwo)
{
  int fd[2];
  pipe (fd);

  pid_t child = fork ();
  if (child < 0)
    return;

  if (child == 0)
    {
      // SECOND COMMAND
      dup2 (fd[0], STDIN_FILENO);
      char *arguments = strdup (commandTwo); 
      char *token = strtok (commandTwo, " ");
      arguments = &arguments[strlen (token) + 1];
      printf ("command2: [%s], arguments: [%s]\n", token, arguments);
      runCmd(token, arguments, fd);
      // FIRST COMMAND
      arguments = strdup (commandOne); 
      token = strtok (commandOne, " ");
      arguments = &arguments[strlen (token)];
      printf ("command: [%s], arguments: [%s]\n", token, arguments);
      runCmd (token, arguments, fd);
      char buffyTheVampSlayer[1000];
      for (int i = 0; i < sizeof (buffyTheVampSlayer); i++)
        buffyTheVampSlayer[i] = '\0';
      printf ("waiting to read\n");
      read (fd[0], buffyTheVampSlayer, sizeof (buffyTheVampSlayer));
      printf ("read\n");
      printf ("%s", buffyTheVampSlayer);
    }
  else
    {
      close (fd[0]);
      close (fd[1]);
    }
}
