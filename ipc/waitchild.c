/*
 * waitchild.c - parent process examines
 * child status, outputs status
 * */
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  pid_t cpid;
  int status;
  int fds[2];

  if (pipe(fds) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((cpid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cpid == 0) { /* child */
    char c;
    close(fds[1]); /* write */
    printf("child is blocked\n");
    if (read(fds[0], &c, 1) < 0) {
      perror("read");
      printf("child exits\n");
      exit(EXIT_FAILURE);
    }
    printf("child exits\n");
    exit(EXIT_SUCCESS);
  }

  close(fds[0]); /* read */

  printf("parent queries child %d status\n", cpid);
  do {
    if (waitpid(cpid, &status, WUNTRACED | WCONTINUED) < 0) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status))
      printf("exited, status=%d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
      printf("killed by signal=%d\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
      printf("stopped by signal=%d\n", WSTOPSIG(status));
    else if (WIFCONTINUED(status))
      printf("continued\n");

    if (write(fds[1], "c", 1) < 0) { /* unblock child */
      perror("write");
      exit(EXIT_FAILURE);
    }
  } while (!WIFEXITED(status) && !WIFSIGNALED(status));

  return 0;
}
