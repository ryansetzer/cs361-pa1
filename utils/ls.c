#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <stdbool.h>
#include <getopt.h>
=======
#include <sys/wait.h>
#include <unistd.h>
>>>>>>> 0a6c1ebe34e2d7850a999d11c60a70f818ce5df1

static void usage (void);
static bool get_args (int, char **, bool, bool);

int
main (int argc, char *argv[])
{
<<<<<<< HEAD
  bool allFiles = false;
  bool listSizes = false;
  if (!get_args (argc, argv, allFiles, listSizes))
    usage ();

  printf ("success\n");
=======

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
>>>>>>> 0a6c1ebe34e2d7850a999d11c60a70f818ce5df1
  return EXIT_SUCCESS;
}

bool
get_args (int argc, char **argv, bool allFiles, bool listSizes)
{
  int i = 0;
  while ((i = getopt (argc, argv, ":a:s")) != -1)
    {
      switch (i)
        {
          case 'a':
            allFiles = true;
          case 's':
            listSizes = true;
          default:
            return false;
        }
    }
  if (optind != argc - 1)
    {
      return false;
    }
  return true;
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
