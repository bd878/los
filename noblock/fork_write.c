/**
 * fork_write.c - parent and child processes
 * write simultaneously to a file
 */
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "../iolib/randstring.h"
#include "../shmem/sync_pipe.h"

static const int LINES_COUNT = 10e2;
static const int LINE_SIZE = 20;
static const int LINE_PREFIX = 20;
static const int NEWLINE_SIZE = 3;

int main() {
  int fd, fd2, cid;
  int fds[2];

  char buf[LINE_PREFIX+LINE_SIZE+NEWLINE_SIZE];
  char prefix[LINE_PREFIX];
  int nwrite = 0;
  int prefix_len = 0;
  int total_write = 0;
  total_write = 0;

  OPEN_PIPE(fds);

  if ((fd = open("./non_block.txt", O_RDWR)) < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if ((fd2 = fcntl(fd, F_DUPFD, 0)) < 0) {
    perror("fnctl");
    exit(EXIT_FAILURE);
  }

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid == 0) { /* child */
    close(fds[1]); /* write end */

    if ((prefix_len = sprintf(prefix, "[%d] ", getpid())) < 0) {
      perror("sprintf");
      exit(EXIT_FAILURE);
    }

    if (strcpy(buf, prefix) < 0) {
      perror("strcpy");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < LINES_COUNT/2; i++) {
      if (read_rand_string(&buf[prefix_len], LINE_SIZE) < 0) {
        fprintf(stderr, "[%d] failed to read random bytes to buffer\n", getpid());
        exit(EXIT_FAILURE);
      }

      if (strcat(buf, "\n") < 0) {
        perror("strcat");
        exit(EXIT_FAILURE);
      }

      nwrite = write(fd2, buf, strlen(buf));
      if (nwrite > 0)
        total_write += nwrite;
    }

    printf("[%d] written bytes: %d\n", getpid(), total_write);
    printf("[%d] waiting parent...\n", getpid());
    WAIT_PARENT(fds);

    exit(EXIT_SUCCESS);
  }
  /* parent */

  close(fds[0]); /* read end */
  printf("[%d] child process: %d\n", getpid(), cid);

  if ((prefix_len = sprintf(prefix, "[%d] ", getpid())) < 0) {
    perror("sprintf");
    exit(EXIT_FAILURE);
  }

  if (strcpy(buf, prefix) < 0) {
    perror("strcpy");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < LINES_COUNT/2; i++) {
    if (read_rand_string(&buf[prefix_len], LINE_SIZE) < 0) {
      fprintf(stderr, "[%d] failed to read random bytes to buffer\n", getpid());
      exit(EXIT_FAILURE);
    }

    if (strcat(buf, "\n") < 0) {
      perror("strcat");
      exit(EXIT_FAILURE);
    }

    nwrite = write(fd, buf, strlen(buf));
    if (nwrite > 0)
      total_write += nwrite; 
  }

  printf("[%d] written bytes: %d\n", getpid(), total_write);
  TELL_CHILD(fds);

  if (waitpid(cid, NULL, 0) < 0) {
    fprintf(stderr, "[%d] waitpid failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

