/*
 * countlinks.c - creates hard link for a given file,
 * verifies, that inode nums are equal
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("not enough args\n");
    exit(1);
  }

  char *filename;
  char *linkname;
  struct stat buf;
  struct stat linkbuf;

  filename = argv[1];

  linkname = malloc(strlen(filename) + strlen(".link") + 1);
  if (linkname == NULL) {
    perror("malloc");
    exit(1);
  }
  printf("linkname size: %d\n", sizeof(linkname));
  strcpy(linkname, filename);
  strcat(linkname, ".link");

  if ((link(filename, linkname)) < 0) {
    perror("link");
    exit(1);
  }

  printf("%s->%s\n", linkname, filename);

  if (stat(filename, &buf) < 0) {
    perror("stat");
    exit(1);
  }

  if (stat(linkname, &linkbuf) < 0) {
    perror("stat linkname");
    exit(1);
  }

  printf("%s: %d\n", filename, buf.st_ino);
  printf("%s: %d\n", linkname, linkbuf.st_ino);

  if (buf.st_ino != linkbuf.st_ino) {
    printf("links are not equal!\n");
    exit(1);
  }

  printf("links are equal\n");

  return 0;
}
