/*
 * linestream.c - returns a FILE* stream of random
 * strings, limits data by sz bytes
 */
#include <stdlib.h>

#include "linestream.h"

FILE* new_line_stream(off_t sz) {
  char buf[MAXLINE];
  FILE* f;

  if ((f = fopen(filename, "a")) == NULL) {
    perror("fopen");
    exit(1);
  }

  strncpy(buf, "hello, world\n", sizeof(buf));

  if (fputs(buf, f) == EOF) {
    perror("fputs");
    exit(1);
  }

  return 0;
}
