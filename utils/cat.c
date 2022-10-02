#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

static void usage (void);
static bool get_args (int argc, char **argv, FILE **script);

#define LINELEN 1024


int
main (int argc, char *argv[])
{
  FILE *script = NULL;
  size_t lineSize = LINELEN;
  char *buffer;

  if (!get_args (argc, argv, &script))
  {
    usage ();
    return EXIT_FAILURE;
  }
  if (script == NULL)
  {
    printf (" ");
    return EXIT_FAILURE;
  } 
  while (getline(&buffer, &lineSize, script) != -1)
  {
    printf ("%s", buffer);
  }
  return EXIT_SUCCESS;
}

static bool
get_args (int argc, char **argv, FILE **script)
{
  if (argc < 2)
      return false;
  *script = fopen (argv[optind], "r");
  return true;
}


static void
usage (void)
{
  printf ("cat, print the contents of a file\n");
  printf ("usage: cat FILE\n");
}
