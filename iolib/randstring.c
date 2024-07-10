/**
 * randstring.c - generates random string
 * of specified size.
 * Generated string contains only
 * ASCII printable symbols
 */
#include <stdio.h>

#define TO_ASCII(point) ((point) % 93 + 33)

int read_rand_string(char *buf, size_t sz) {
  FILE* fs;
  size_t written;

  if ((fs = fopen("/dev/urandom", "r")) == NULL) {
    perror("fopen");
    return -1;
  }

  written = fread(buf, sizeof(char), sz-1, fs);
  if (written != sz) {
    if (ferror(fs) != 0) {
      printf("error reading from /deb/urandom");
      return -1;
    }
  }

  for (size_t i = 0; i < sz; i++) {
    unsigned long val = buf[i];
    buf[i] = TO_ASCII(val);
  }

  buf[sz-1] = '\0';

  fclose(fs);
  return written+1;
}
