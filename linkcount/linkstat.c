/*
 * linkstat.c - print links count on file
 */
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  char *filename = argv[1];
  
  struct stat buf;
  if (stat(filename, &buf) < 0) {
    perror("stat");
    exit(1);
  }

  printf("%s links: %d\n", filename, buf.st_nlink);
  return 0;
}
