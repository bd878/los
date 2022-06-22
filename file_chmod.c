#include "lib.h"
#include <fcntl.h>

int
main(void)
{
  struct stat statbuf;

  if (stat("foo", &statbuf) < 0) {
    printf("stat call error for file foo\n");
    exit(1);
  }

  if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
    printf("chmod call error for file foo\n");
    exit(1);
  }

  if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
    printf("chmod call error for file bar\n");
    exit(1);
  }

  exit(0);
}
