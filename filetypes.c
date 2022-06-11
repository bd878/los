#include "lib.h"

/**
 * Per each file outputs its type
 *
 */

int
main(int argc, char *argv[])
{
  int i;
  struct stat buf;
  char *ptr;

  for (i = 0; i < argc; i++) {
    printf("%s: ", argv[i]);
    if (lstat(argv[i], &buf) < 0) {
      printf("cannot call lstat\n");
      continue;
    }
    if (S_ISREG(buf.st_mode))
      ptr = "regular file";
    else if (S_ISDIR(buf.st_mode))
      ptr = "directory";
    else if (S_ISCHR(buf.st_mode))
      ptr = "symbol device file";
    else if (S_ISBLK(buf.st_mode))
      ptr = "block device file";
    else if (S_ISFIFO(buf.st_mode))
      ptr = "FIFO";
    else if (S_ISLNK(buf.st_mode))
      ptr = "symbol link";
    else if (S_ISSOCK(buf.st_mode))
      ptr = "socket";
    else
      ptr = "** unknown file type **";

    printf("%s\n", ptr);
  }

  exit(0);
}
