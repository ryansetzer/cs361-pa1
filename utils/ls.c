#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>    
#include <fcntl.h>

static void usage (void);
static bool get_args (int, char **, bool *, bool *, bool *, char **);

int
main (int argc, char *argv[])
{
  bool allFiles = false;
  bool listSizes = false;
  bool badFlag = false;
  bool usingHome = false;
  char *dir;
  DIR *d;
  struct dirent *entry;
  if (!get_args (argc, argv, &allFiles, &listSizes, &badFlag, &dir))
  {
    if (!badFlag)
    {
      usage ();
    }
    else
      printf ("");
    return EXIT_FAILURE;
  }
  if (dir == NULL)
    {
      d = opendir ("."); // uses current directory
      usingHome = true;
    }
  else
    d = opendir (dir); // uses given directory
  if (d == NULL)
    {
      printf ("");
      return EXIT_FAILURE;
    }
  entry = readdir (d);
  struct stat st;
  while (entry != NULL)
    {
      char buffer[1000];
      for (int i = 0; i < sizeof (buffer); i++)
        buffer [i] = ' ';
      char *filename = entry -> d_name;
      getcwd (buffer, sizeof (buffer));
      char *fullDir = strcat (buffer, "/");
      if (!usingHome)
        {
          fullDir = strcat (fullDir, dir);
          fullDir = strcat (fullDir, "/");
        }
      fullDir = strcat (fullDir, filename);
      lstat (fullDir, &st);
      if (allFiles && listSizes)
        printf ("%ld %s\n", st.st_size, entry -> d_name);
      else if (allFiles && !listSizes)
        printf ("%s\n", entry -> d_name);
      else if (!allFiles && listSizes)
        {
          if ((entry -> d_name)[0] != '.')
            printf ("%ld %s\n", st.st_size, entry -> d_name);
        }
      else if (!allFiles && !listSizes)
        {
          if ((entry -> d_name)[0] != '.')
            printf ("%s\n", entry -> d_name);
        }
      else
        printf ("Some'than ain't rite\n");
      entry = NULL;
      entry = readdir (d);
    }
  return EXIT_SUCCESS;
}

bool
get_args (int argc, char **argv, bool *allFiles, bool *listSizes, bool *badFlag, char **dir)
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
            *badFlag = true;
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
