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

  printf ("success\n");
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
