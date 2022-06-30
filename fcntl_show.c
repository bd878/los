#include "lib.h"

/*
 * Receives file descriptor
 * and outputs fd flags
 **/
int
main(int argc, char *argv[])
{
  int val;

  if (argc != 2) {
    printf("Usage: a.out <fd>\n");
    exit(1);
  }

  if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
    printf("fcntl for %d failed", atoi(argv[1]));
    exit(1);
  }

  switch (val & O_ACCMODE) {
    case O_RDONLY:
      printf("read only\n");
      break;

    case O_WRONLY:
      printf("write only\n");
      break;

    case O_RDWR:
      printf("read and write\n");
      break;

    default:
      printf("unknown access mode\n");
      exit(1);
  }

  if (val & O_APPEND)
    printf(", append");

  if (val & O_NONBLOCK)
    printf(", nonblock mode");

  if (val & O_SYNC)
    printf(", sync write mode");

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
  if (val & O_FSYNC)
    printf(", sync write mode");
#endif

  putchar('\n');
  exit(0);
}
