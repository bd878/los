#include "lib.h"
#include <dirent.h>

/*
* Tests if two paths points
* to same directories.
* Does not follow symlinks so far
*
**/

int
main(int argc, char* argv[])
{
  struct stat buf1, buf2;

  if (argc != 3) {
    printf("Usage: same_dirs_test dir1 dir2");
    exit(1);
  }

  if (lstat(argv[1], &buf1) < 0) {
    printf("cannot call stat for the first file: %s\n", argv[1]);
    exit(1);
  }

  if (lstat(argv[2], &buf2) < 0) {
    printf("cannot call stat for the second file: %s\n", argv[2]);
    exit(1);
  }

  if (buf1.ino_t == buf2.ino_t)
    printf("it is the same file\n");
  else
    printf("these are two differend files\n");

  exit(0);
}
