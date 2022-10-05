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

int runCmd (char *command)
{
  // Creating Pipe
  int fd[2];
  pipe (fd);
  // Parsing Arguments
  char *cmd1 = strtok (command, "|");
  char *cmd2 = strtok (NULL, "\n");
  // Creating Possible Runnable Commands
  char *firstCmd = strtok (cmd1, " ");
  char *firstArgs = strtok (NULL, "\n");
  char *secondCmd = strtok (cmd2, " ");
  char *secondArgs = strtok (NULL, "\n");
  
  if (cmd2 == NULL && firstArgs == NULL)
    firstCmd [strlen (firstCmd) - 1] = '\0';
  //printf("first-%s\nargs1-%s\nsecond-%s\nargs2-%s\n", firstCmd, firstArgs, secondCmd, secondArgs);

  // ┌────────────────┐
  // | SINGLE COMMAND |
  // └────────────────┘
  if (cmd2 == NULL) // No Pipe Found (Single Command)
    {
      pid_t child_pid = fork();
      if (child_pid < 0) // Bad Fork
        return EXIT_FAILURE;
      // CHILD
      if (child_pid == 0)
        {
          close (fd[0]); // closing read end of pipe
          dup2 (fd[1], STDOUT_FILENO); // redirecting stdout to write end of pipe
          char *arg1= strtok (firstArgs, " ");
          char *arg2 = strtok (NULL, " ");
          char *arg3 = strtok (NULL, "\\n");
          if (firstArgs == NULL)
          {
            execlp (firstCmd, firstCmd, NULL);
          }
          else if (arg1 != NULL && arg2 == NULL && arg3 == NULL)
            execlp (firstCmd, firstCmd, arg1, NULL);
          else if (arg1 != NULL && arg2 != NULL && arg3 != NULL)
            execlp (firstCmd, firstCmd, arg1, arg2, arg3, NULL);
          else
            execlp (firstCmd, firstCmd, arg1, arg2, NULL);
        }
      // PARENT
      else
        {
          close (fd[1]); // close write end of pipe
          char buffer[1000];
          for (int i = 0; i < sizeof (buffer); i++)
            buffer[i] = '\0';
          read (fd[0], buffer, sizeof (buffer));
          if (buffer[0] != ' ')
            printf ("%s", buffer);
        }
        return EXIT_SUCCESS;
    }
  // ┌──────────────┐
  // | TWO COMMANDS |
  // └──────────────┘
    else
      {
        int fd2[2];
        pipe (fd2);
        pid_t child_pid = fork();
        if (child_pid < 0) // Bad Fork
          return EXIT_FAILURE;
        // CHILD
        if (child_pid == 0)
          {
            pid_t child2_pid = fork();

            if (child2_pid == 0)
              {
                // Running Second Command First
                close (fd[1]);
                close (fd2[0]);
                dup2 (fd[0], STDIN_FILENO); // redirecting stdin to read end of pipe (first command)
                dup2 (fd2[1], STDOUT_FILENO); // redirecting stdout to write end of pipe
                char *arg1= strtok (secondArgs, " ");
                char *arg2 = strtok (NULL, " ");
                char *arg3 = strtok (NULL, "\\n");
                if (secondArgs == NULL)
                {
                  execlp (secondCmd, secondCmd, NULL);
                }
                else if (arg1 != NULL && arg2 == NULL && arg3 == NULL)
                  execlp (secondCmd, secondCmd, arg1, NULL);
                else if (arg1 != NULL && arg2 != NULL && arg3 != NULL)
                  execlp (secondCmd, secondCmd, arg1, arg2, arg3, NULL);
                else
                  execlp (secondCmd, secondCmd, arg1, arg2, NULL);
              }
            // PARENT
            else
              {
                // Running First Command Second
                close (fd2[0]);
                close (fd2[1]);
                close (fd[0]); // close read end of pipe
                dup2 (fd[1], STDOUT_FILENO); // redirecting stdout to write end of pipe
                printf ("HELLO\n");
                char *arg1= strtok (secondArgs, " ");
                char *arg2 = strtok (NULL, " ");
                char *arg3 = strtok (NULL, "\\n");
                if (secondArgs == NULL)
                {
                  execlp (secondCmd, secondCmd, NULL);
                }
                else if (arg1 != NULL && arg2 == NULL && arg3 == NULL)
                  execlp (secondCmd, secondCmd, arg1, NULL);
                else if (arg1 != NULL && arg2 != NULL && arg3 != NULL)
                  execlp (secondCmd, secondCmd, arg1, arg2, arg3, NULL);
                else
                  execlp (secondCmd, secondCmd, arg1, arg2, NULL);
              }
          }      // PARENT
      else
        {
          close (fd[0]);
          close (fd[1]);
          close (fd2[1]); // close write end of pipe
          char buffer[1000];
          for (int i = 0; i < sizeof (buffer); i++)
            buffer[i] = '\0';
          read (fd2[0], buffer, sizeof (buffer));
          if (buffer[0] != ' ')
            printf ("%s", buffer);
        }
        return EXIT_SUCCESS;
      }
}
