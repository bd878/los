#include "lib.h"

void
set_fl(int fd, int flags)
{
  int val;

  if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
    printf("failed to execute F_GETFL of fcntl\n");
    exit(1);
  }

  val |= flags; /* turn on flags */

  if (fcntl(fd, F_SETFL, val) < 0) {
    printf("failed to execute F_SETFL of fcntl\n");
    exit(1);
  }
}

void
clr_fl(int fd, int flags)
{
  int val;

  if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
    printf("failed to execute F_GETFL of fcntl\n");
    exit(1);
  }

  val &= flags;

  if (fcntl(fd, F_SETFL, val) < 0) {
    printf("failed to execute F_SETFL of fcntl\n");
    exit(1);
  }
}
