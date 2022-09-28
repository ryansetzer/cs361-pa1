#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

static void usage (void);
static bool get_args (int, char **, bool, bool);

int
main (int argc, char *argv[])
{
  bool allFiles = false;
  bool listSizes = false;
  if (!get_args (argc, argv, allFiles, listSizes))
    usage ();

  int fd[2];
//  pipe (fd);


//  pid_t child = fork();
//  if (child < 0)
//    return 1;
//  if (child == 0)
//    {
//      close (fd[0]); // close read end of pipe
//      dup2 (STDOUT_FILENO, fd[1]);
//      if (allFiles && listSizes)
//        execlp ("ls", "ls", "a", "s");
//      else if (allFiles && !listSizes)
//        execlp ("ls", "ls", "a");
//      else if (!allFiles && listSizes)
//        execlp ("ls", "ls", "s");
//      else
//        execlp ("ls", "ls");      
//  char buffer[1000];
//  close (fd[1]); // close write end of pipe
// read (fd[0], buffer, sizeof(buffer));
//  printf ("%s\n", buffer); 
//  printf ("success\n");
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
