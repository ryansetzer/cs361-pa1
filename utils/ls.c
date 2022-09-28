#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>                                                                                                                                                                                                 
#include <dirent.h>    

static void usage (void);
static bool get_args (int, char **, bool *, bool *, char **);

int
main (int argc, char *argv[])
{
  bool allFiles = false;
  bool listSizes = false;
  char *dir;
  DIR *d;
  if (!get_args (argc, argv, &allFiles, &listSizes, &dir))
    usage ();

  printf("dir=%s\n", dir);
  if (dir == NULL)
    d=opendir(".");
  else
    d=opendir(dir);
  return EXIT_SUCCESS;
}

bool
get_args (int argc, char **argv, bool *allFiles, bool *listSizes, char **dir)
{
  int i = 0;
  while ((i = getopt (argc, argv, "as")) != -1)
    {
      switch (i)
        {
          case 'a':
            *allFiles = true;
	    break;
          case 's':
            *listSizes = true;
	    break;
          default:
            return false;
        }
    }
  if (optind > argc)
    {
      return false;
    }

  *dir = argv[optind];
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
