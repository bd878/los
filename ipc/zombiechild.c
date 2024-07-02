/*
 * zombiechild.c - child exits, but parent process
 * does not waitpid it.
 * */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
  pid_t cpid;
  pid_t pid;

  if ((cpid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cpid == 0) { /* child */
    pid_t ppid;
    for (int i = 0; i < 5; i++) {
      ppid = getppid();
      printf("i=%d, ppid=%d\n", i+1, ppid);
      sleep(5);
    }
    exit(EXIT_SUCCESS);
  }
  /* parent */
  pid = getpid();
  printf("pid=%d, cpid=%d\n", pid, cpid);
  return 0;
}
