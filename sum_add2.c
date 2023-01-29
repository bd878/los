#include "lib.h"

static void sig_pipe(int); /* sig handler */

int
main(void)
{
  int n, fd1[2], fd2[2];
  pid_t pid;
  char line[MAXLINE];

  if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
    err_sys("failed to call signal");

  if (pipe(fd1) < 0 || pipe(fd2) < 0)
    err_sys("failed to call pipe");

  if ((pid = fork()) < 0) {
    err_sys("failed to call fork");
  } else if (pid > 0) { /* parent proc */
    close(fd1[0]);
    close(fd2[1]);

    while (fgets(line, MAXLINE, stdin) != NULL) {
      n = strlen(line);
      if (write(fd1[1], line, n) != n)
        err_sys("failed to write to channel");
      if ((n = read(fd2[0], line, MAXLINE)) < 0)
        err_sys("failed to read from channel");
      if (n == 0) {
        err_msg("channel was closed in a forked proc");
        break;
      }
      line[n] = 0;
      if (fputs(line, stdout) == EOF)
        err_sys("failed to call fputs");
    }

    if (ferror(stdin))
      err_sys("failed to get data from stdin");
    exit(0);
  } else { /* child proc */
    close(fd1[1]);
    close(fd2[0]);
    if (fd1[0] != STDIN_FILENO) {
      if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
        err_sys("failed to call dup2 for stdin");
      close(fd1[0]);
    }

    if (fd2[1] != STDOUT_FILENO) {
      if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
        err_sys("failed to call dup2 for stdout");
      close(fd2[1]);
    }
    if (execl("./add2", "add2", (char *)0) < 0)
      err_sys("failed to call execl");
  }
  exit(0);
}

static void
sig_pipe(int signo)
{
  printf("Caught SIGPIPE signal\n");
  exit(1);
}