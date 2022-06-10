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
  if (argc != 3) {
    printf("Usage: same_dirs_test dir1 dir2");
    exit(1);
  }
}
