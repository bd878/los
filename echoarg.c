#include "lib.h"

/*
 * Prints command-line arguments
 * According to POSIX,
 * argv[argc] == NULL
 **/
int
main(int argc, char *argv[])
{
  for (int i = 0; argv[i] != NULL; i++)
    printf("argv[%d]: %s\n", i, argv[i]);
  exit(0);
}
