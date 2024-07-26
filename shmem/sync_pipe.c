/**
 * sync_pipe.c - implement sync_pipe.h
 * primitivies to synchronize parent & child
 * processes
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void
OPEN_PIPE(int fd[2])
{
  if (pipe(fd) < 0) {
    fprintf(stderr, "[%d] failed to open pipe: %s\n",
      getpid(), strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void
TELL_PARENT(int fd[2])
{
  if (write(fd[1], "c", 1) != 1) {
    fprintf(stderr, "[%d] failed to tell parent: %s\n",
      getpid(), strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void
WAIT_PARENT(int fd[2])
{
  char c;

  if (read(fd[0], &c, 1) != 1) {
    fprintf(stderr, "[%d] failed to wait parent: %s\n",
      getpid(), strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (c != 'p') {
    fprintf(stderr, "[%d] wrong data, got %c, expected %c\n",
      getpid(), c, 'p');
    exit(EXIT_FAILURE);
  }
}

void
WAIT_CHILD(int fd[2])
{
  char c;

  if (read(fd[0], &c, 1) != 1) {
    fprintf(stderr, "[%d] failed to wait child: %s\n",
      getpid(), strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (c != 'c') {
    fprintf(stderr, "[%d] wrong data, got %c, expected %c\n",
      getpid(), c, 'c');
    exit(EXIT_FAILURE);
  }
}

void
TELL_CHILD(int fd[2])
{
  if (write(fd[1], "p", 1) != 1) {
    fprintf(stderr, "[%d] failed to tell child: %s\n",
      getpid(), strerror(errno));
    exit(EXIT_FAILURE);
  }
}
