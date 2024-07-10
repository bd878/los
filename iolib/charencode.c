/**
 * charencode.c - encode byte to char,
 * output to stdout
 */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("argc < 1\n");
    exit(-1);
  }

  unsigned long val = strtoll(argv[1], NULL, 10);
  if (errno == ERANGE) {
    if (val == LONG_MAX) {
      fprintf(stderr, "beyond the range");
      exit(-1);
    }

    if (val == LONG_MIN) {
      fprintf(stderr, "under the range");
      exit(-1);
    }

    perror("strtol");
    exit(-1);
  }

  val = val % 93 + 33;
  printf("%u : %c\n", val, val);
  return 0;
}
