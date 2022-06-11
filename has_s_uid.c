#include "lib.h"

/**
 * Explores given file
 * for sticky bit set 
 *
 */

int
main(int argc, char *argv[])
{
  struct stat buf;

  if (argc != 2) {
    printf("Usage: has_s_uid file");
    exit(1);
  }

  if (stat(argv[1], &buf) < 0) {
    printf("cannot call stat for given file\n");
    exit(1);
  }

  if (buf.st_mode & S_ISUID)
    printf("st uid is set\n");
  else
    printf("st uid is not set\n");

  if (buf.st_mode & S_ISGID)
    printf("st gid is set\n");
  else
    printf("st gid is not set\n");

  exit(0);
}
 
