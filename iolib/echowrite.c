/**
 * echowrite.c - write random strings to a file
 * using standard io lib.
 * Measures time consumed
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

#include "randstring.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define _FILE_NAME_SIZE 20
#define _LINES_COUNT 100000
#define _LINE_LENGTH 200

static void pr_times(clock_t);

int main() {
  FILE* fs;
  char fname[_FILE_NAME_SIZE], buf[_LINE_LENGTH];
  clock_t start, end;
  struct tms tmstart, tmend;
  time_t curt;

  if ((curt = time(NULL)) < 0) {
    perror("time");
    exit(1);
  }

  if (snprintf(fname, _FILE_NAME_SIZE, "test_%d.txt", curt) < 0) {
    perror("snprintf");
    exit(1);
  }

  printf("file: %s\n", fname);

  if ((fs = fopen(fname, "a")) == NULL) {
    perror("fopen");
    exit(1);
  }

  if ((start = times(&tmstart)) < 0)
    fprintf(stderr, "failed to obtain start time stat\n");

  for (int i = 0; i < _LINES_COUNT; i++) {
    if (read_rand_string(buf, ARRAY_SIZE(buf)) < 0) {
      perror("read_rand_string");
      exit(1);
    }

    if (fputc('\n', fs) < 0) {
      perror("fputc");
      exit(1);
    }

    if (fwrite(buf, sizeof(char), ARRAY_SIZE(buf), fs) < 0) {
      perror("fwrite");
      exit(1);
    }
  }

  fclose(fs);

  if ((end = times(&tmend)) < 0)
    fprintf(stderr, "failed to obtain end time stat\n");

  pr_times(end - start);

  printf("done.\n");
  exit(0);
}

static void pr_times(clock_t real)
{
  static long clock_ticks = 0; 
  if (clock_ticks == 0)
    if ((clock_ticks = sysconf(_SC_CLK_TCK)) < 0) {
      perror("sysconf");
      exit(-1);
    }

  printf("real time: %7.9f\n", real / (double) clock_ticks);
}
