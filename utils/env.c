#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "/cs/home/stu/setzerrj/cs361-pa1/src/hash.h"

static void usage (void);

int
main (int argc, char *argv[], char *envp[])
{
  //hash_init (1000);
  if (argc == 0) // Bad Arguments
    {
      usage();
      return EXIT_FAILURE;
    }
  // Determining Executable
  char *executable = argv[argc - 1];
  bool isExecutable = executable[0] == '.';
  for (int i = 0; i < argc; i++)
    {
      //printf ("argv[%d] = [%s]\n", i, argv[i]);
      // Parsing Key and Value from argv[1];
      char *key = strtok (argv[i], "=");
      char *value = strtok (NULL, "=");
      //hash_insert (key, value);
      if (isExecutable)
        {
        // Checking if param passed is a valid key-value pair
        if (key != NULL && value != NULL)
          {
            printf ("%s=%s\n", key, value);
          }
        }
    }
  if (isExecutable) // Run 2nd Program w/ temp env vars
    {

    }
  //hash_destroy();
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("env, set environment variables and execute program\n");
  printf ("usage: env [name=value ...] PROG ARGS\n");
}

