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
  struct dirent *entry;
  if (!get_args (argc, argv, &allFiles, &listSizes, &dir))
    usage ();
  if (dir == NULL)
    d = opendir ("."); // uses current directory
  else
    d = opendir (dir); // uses given directory
  if (d == NULL)
    {
      printf (" ");
      return EXIT_FAILURE;
    }
  entry = readdir (d);
  while (entry != NULL)
    {
      if (allFiles && listSizes)
        printf ("%d %s   ", entry -> d_reclen, entry -> d_name);
      else if (allFiles && !listSizes)
        printf ("%s  ", entry -> d_name);
      else if (!allFiles && listSizes)
        {
          if ((entry -> d_name)[0] != '.')
            printf ("%d %s   ", entry -> d_reclen, entry -> d_name);
        }
      else if (!allFiles && !listSizes)
        {
          if ((entry -> d_name)[0] != '.')
            printf ("%s  ", entry -> d_name);
        }
      else
        printf ("Some'than ain't rite\n");
      entry = readdir (d);
    }
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
