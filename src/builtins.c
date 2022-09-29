#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "process.h"
#include "hash.h"

// Given a message as input, print it to the screen followed by a
// newline ('\n'). If the message contains the two-byte escape sequence
// "\\n", print a newline '\n' instead. No other escape sequence is
// allowed. If the sequence contains a '$', it must be an environment
// variable or the return code variable ("$?"). Environment variable
// names must be wrapped in curly braces (e.g., ${PATH}).
//
// Returns 0 for success, 1 for errors (invalid escape sequence or no
// curly braces around environment variables).
int
echo (char *message)
{
  if (message != NULL)
    {
      message[strlen (message) - 1] = '\0';
    } 
  char *token = strtok_r (message, "\\n", &message);
  char *token2;
  do
    {
      token2 = strtok_r (token, "$", &token);
      printf ("%s", token2);
      token2 = strtok_r (token, "$", &token);
      if (token2 != NULL)
        {
          token2[strlen (token2) - 1] = '\0';
          char* value = hash_find (&token2[1]);
          // printf ("key: [%s] value: [%s]\n", &token2[1], value);
          if (value != NULL)
          	printf ("%s", value);
        }
      printf ("\n");
      token = strtok_r (message, "\\n", &message);
    } while (token != NULL);
  return 0;
}
// Given a key-value pair string (e.g., "alpha=beta"), insert the mapping
// into the global hash table (hash_insert ("alpha", "beta")).
//
// Returns 0 on success, 1 for an invalid pair string (kvpair is NULL or
// there is no '=' in the string).
int
export (char *kvpair)
{
  char *token = strtok (kvpair, "=");
  char *key = token;
  token = strtok (NULL, "=");
  char *value = token;
  value [strlen (value) - 1] = '\0';
  // printf ("key: [%s] value: [%s]\n", key, value);
  hash_insert (key, value);
  return 0;
}

// Prints the current working directory (see getcwd()). Returns 0.
int
pwd (void)
{
  char dir[1000];
  if (getcwd(dir, sizeof(dir)) != NULL)
    printf ("%s\n", dir);
  return 0;
}

// Removes a key-value pair from the global hash table.
// Returns 0 on success, 1 if the key does not exist.
int
unset (char *key)
{
  hash_remove (key);
  return 0;
}

// Given a string of commands, find their location(s) in the $PATH global
// variable. If the string begins with "-a", print all locations, not just
// the first one.
//
// Returns 0 if at least one location is found, 1 if no commands were
// passed or no locations found.
int
which (char *cmdline)
{
  if (cmdline == NULL)
    return 1;
  else
    for (int i = 0; i < strlen (cmdline); i++)
      {
        if (cmdline[i] == ' ' || cmdline[i] == '\n')
          {
            cmdline[i] = '\0';
            break;
          }
      }
  if (isBuiltIn (cmdline))
  {
    printf ("%s: dukesh built-in command\n", cmdline); 
    return 0;  
  }
  else if (strncmp (cmdline, "./", 2) == 0)
  {
    if (isExecutable (&cmdline[3]))
      printf ("%s: is executable\n", cmdline);
    printf ("%s\n", cmdline);
    return 0;
  }
  getcwd (PATH, sizeof (PATH));
  printf("%s/%s\n", PATH, cmdline);
  return 0;
}
// Changes the current working directory.
// Returns 0 for success, 1 for errors.
int
cd (char *newDir)
{
  if (newDir == NULL)
    return 1;
  newDir [strlen (newDir) - 1] = '\0';
  if (chdir (newDir) != 0)
    perror ("cd failed");
  strcpy (PATH, newDir);
  return 0;
}
