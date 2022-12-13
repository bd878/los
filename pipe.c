#include "lib.h"

/*
 * Pass data through pipe
 * from parent process
 * to child
 **/

int
main(void)
{
  int n;
  int fd[2];
  pid_t pid;
  char line[MAXLINE];

  if (pipe(fd) < 0) {
    err_sys("failed to call pipe");
  }

  if ((pid = fork()) < 0) {
    err_sys("failed to call fork");
  } else if (pid > 0) { /* parent process */
    close(fd[0]);
    write(fd[1], "hello, world\n", 11);
  } else {  /* child process */
    close(fd[1]);
    n = read(fd[0], line, MAXLINE);
    write(STDOUT_FILENO, line, n);
  }
  exit(0);
}
