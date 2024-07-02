/*
 * forkpipe.c - synchronize parent and child through
 * named pipe
 */
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main() {
  pid_t cpid;
  char c;
  int fds[2];
  struct stat sb;

  if (pipe(fds) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (fstat(fds[0], &sb) < 0) {
    perror("fstat sb1");
    exit(EXIT_FAILURE);
  }

  if (S_ISFIFO(sb.st_mode))
    printf("named pipe\n");

  if ((cpid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cpid == 0) { /* child */
    close(fds[0]); /* read end */
    for (int i = 0; i < 5; i++) {
      printf("child %d\n", i+1);
      sleep(1);
    }
    if (write(fds[1], "c", 1) < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    printf("child\n");
    fflush(stdout);
    exit(EXIT_SUCCESS);
  }

  /* parent */
  close(fds[1]); /* write end */
  printf("parent slipping...\n");
  if (read(fds[0], &c, 1) < 0) { /* will block */
    perror("read");
    exit(EXIT_FAILURE);
  }
  printf("parent\n");
  return 0;
}
