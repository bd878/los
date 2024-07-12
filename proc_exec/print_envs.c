/**
 * print_envs.c - outputs environment variables.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  extern char** environ;
  char** var;
  FILE* fp;
  char* me;
  char* fname;

  if (argc != 2) {
    printf("Usage:%s <out-file>\n", argv[0]);
    exit(1);
  }

  me = argv[0];
  fname = argv[1];

  printf("[%s] write envs to file %s\n", me, fname);

  if ((fp = fopen(fname, "w")) == NULL) {
    perror("fopen");
    exit(1);
  }

  printf("[%s] pid: %d\n", me, getpid());
  printf("[%s] ppid: %d\n", me, getppid());

  for (var = environ; *var != NULL; var += 1) {
    fprintf(fp, "%s\n", *var);
  }

  fclose(fp);

  return 0;
}
