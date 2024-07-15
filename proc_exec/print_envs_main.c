/**
 * print_envs_main.c - outputs environment variables.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printenvs.h"

int main(int argc, char** argv) {
  extern char** environ;
  char** var;
  char* me;
  char* fname;

  if (argc != 2) {
    printf("Usage:%s <out-file>\n", argv[0]);
    exit(1);
  }

  me = argv[0];
  fname = argv[1];

  printf("[%s] write envs to file %s\n", me, fname);

  printf("[%s] pid: %d\n", me, getpid());
  printf("[%s] ppid: %d\n", me, getppid());

  if (printenvs(fname) < 0) {
    perror("printenvs");
    exit(EXIT_FAILURE);
  }

  return 0;
}
