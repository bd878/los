/**
 * fd_non_block.c - use fcntl to set fd O_NONBLOCK
 */
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_fd_flags(int);

int main() {
  int fd;
  int flags;

  if ((fd = open("./non_block.txt", O_CREAT, 0666)) < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  printf("fd: %d\n", fd);
  print_fd_flags(fd);

  flags |= O_NONBLOCK;

  if ((flags = fcntl(fd, F_SETFL, flags)) < 0) {
    fprintf(stderr, "[%d] failed to set O_NONBLOCK to fd\n", getpid());
    exit(EXIT_FAILURE);
  }

  printf("[%d] set O_NONBLOCK to fd\n", getpid());
  print_fd_flags(fd);
  exit(EXIT_SUCCESS);
}

void print_fd_flags(int fd) {
  int flags;

  if ((flags = fcntl(fd, F_GETFL)) < 0) {
    perror("fcntl");
    exit(EXIT_FAILURE);
  }

  printf("[%d] fd flags: %d\n", getpid(), flags);
}
