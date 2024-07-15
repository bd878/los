/**
 * printenvs.c - outputs environment variables.
 */
#include <stdio.h>
#include "printenvs.h"

int printenvs(const char* fname) {
  extern char** environ;

  char** var;
  FILE* fp;

  if ((fp = fopen(fname, "w")) == NULL) {
    fprintf(stderr, "failed to open file %s\n", fname);
    return -1;
  }

  for (var = environ; *var != NULL; var += 1) {
    fprintf(fp, "%s\n", *var);
  }

  fclose(fp);

  return 0;
}
