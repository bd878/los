#include "lib.h"
#include "dedup.h"

static char filename[] = "dedup.txt";

int get_flags(int fd);

/*
 * Opens (or creates) file
 * by provided filename
 * and duplicated fd through custom
 * dedup function.
 * File that fds are point on
 * has same flags and same inode numbers.
 * Compare those flags
 * to see if file is the same
 **/
int
main(void)
{
  int fd, fd2, flags, flags2;

  if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 644)) < 0) {
    printf("failed to open %s file\n", filename);
    exit(1);
  }

  if ((fd2 = dedup(fd, fd + 1)) < 0) {
    printf("failed to duplicate %d fd\n", fd);
    exit(1);
  }

  flags = get_flags(fd);
  flags2 = get_flags(fd2);

  printf("%d fd flags %d\n", fd, flags);
  printf("%d fd2 flags %d\n", fd2, flags2);

  exit(0);
}

int
get_flags(int fd)
{
  int flags;
  if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
    printf("failed to get flags for %d fd\n", fd);
    exit(1);
  }
  return flags;
}
