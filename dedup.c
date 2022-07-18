#include "lib.h"
#include "dedup.h"

int
dedup(int fd, int fd2)
{
  if (fd == fd2) {
    return fd2;
  }

  close(fd2);

  int fds[MAX_FD];
  int i = 0;
  fds[i] = dup(fd);
  while (fds[i] != fd2 && i+1 < MAX_FD) {
    i++;
    if ((fds[i] = dup(fd)) < 0) { /* failed to dup fd */
      close_fds(fds, sizeof(fds), 0);
      return -1;
    }
  }

  close_fds(fds, sizeof(fds), fd2);

  return fd2;
}

void
close_fds(int fds[], size_t size, int except)
{
  for (int i = 0; i < size; i++) {
    if (fds[i] != except) {
      close(fds[i]);
    }
  }
}
