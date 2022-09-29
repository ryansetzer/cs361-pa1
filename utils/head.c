#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <getopt.h>

// You may assume that lines are no longer than 1024 bytes
#define LINELEN 1024

static void usage (void);
static bool get_args (int argc, char **argv, FILE **script, int *num, bool *badFlag);

int
main (int argc, char *argv[])
{
  bool badFlag = false;

  FILE *script = NULL;
  int numLines = 5;
  size_t lineSize = LINELEN;
  char *buffer;
  if (!(get_args (argc, argv, &script, &numLines, &badFlag)))
  {
    if (!badFlag)
    {
      usage ();
    }
    else
      printf (" ");
    return EXIT_FAILURE;
  }
  if (script == NULL)
  {
    printf (" ");
    return EXIT_FAILURE;
  } 
  int i = numLines;
  while (i > 0)
  {
    int eof = getline (&buffer, &lineSize, script);
    if (eof == -1)
      break;
    printf ("%s", buffer);
    i--;
  }
  return EXIT_SUCCESS;
}

static bool
get_args (int argc, char **argv, FILE **script, int *num, bool *badFlag)
{
  int ch = 0;
  while ((ch = getopt (argc, argv, "n:h")) != -1)
    {
      switch (ch)
        {
        case 'n':
          *num = atoi(argv[argc - 2]);
          break;
        default:
          *badFlag = true;
          return false;
        }
    }   
    if (argc < 2)
      return false;
    *script = fopen (argv[argc - 1], "r");

  return true;
}

static void
usage (void)
{
  printf ("head, prints the first few lines of a file\n");
  printf ("usage: head [FLAG] FILE\n");
  printf ("FLAG can be:\n");
  printf ("  -n N     show the first N lines (default 5)\n");
  printf ("If no FILE specified, read from STDIN\n");
}
