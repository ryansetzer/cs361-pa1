#include <stdio.h>
#include <stdlib.h>

static void usage (void);

int
main (int argc, char *argv[], char *envp[])
{
  if (argc == 0)
    usage();
  if (argc == 1)
  {
    for (int i = 0; i < sizeof (&envp); i++)
      printf ("%s\n", envp [i]);
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("env, set environment variables and execute program\n");
  printf ("usage: env [name=value ...] PROG ARGS\n");
}

