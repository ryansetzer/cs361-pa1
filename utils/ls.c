#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void usage (void);

int
main (int argc, char *argv[])
{

  int fd[2];
  pipe (fd);
  pid_t child_pid = fork();
  if (child_pid < 0)
    return EXIT_FAILURE;
  if (child_pid == 0)
    {
      close (fd[0]);
      dup2 (fd[1], STDOUT_FILENO);
      execlp ("ls", "ls");
    }
  close (fd[1]);
  char *buffer[1000];
  read (fd[0], buffer, sizeof (buffer));
  printf ("%s", buffer);
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("ls, list directory contents\n");
  printf ("usage: ls [FLAG ...] [DIR]\n");
  printf ("FLAG is one or more of:\n");
  printf ("  -a       list all files (even hidden ones)\n");
  printf ("  -s       list file sizes\n");
  printf ("If no DIR specified, list current directory contents\n");
}
