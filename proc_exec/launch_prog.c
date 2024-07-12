/**
 * launch-prog.c - exec's a given program
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  pid_t cid;
  int wstatus;

  if (argc != 2) {
    printf("Usage: %s <envs-file>\n", argv[0]);
    exit(1);
  }

  printf("[launch-prog] pid: %d\n", getpid());
  printf("===\n");

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(1);
  } else if (cid == 0) { /* child */
     if (execl("./print_envs.out", "print_envs", "./envs.txt", (char *)0) < 0) {
      perror("execv");
      exit(1);
    }
    /* print_envs.out executes.. */
  }

  if (waitpid(cid, &wstatus, 0) < 0) {
    perror("waitpid");
    exit(1);
  }

  printf("===\n");

  if (WIFEXITED(wstatus)) {
    printf("[launch-prog] child exited normally with status %d\n", WEXITSTATUS(wstatus));
  }

  if (WIFSIGNALED(wstatus)) {
    printf("[launch-prog] child exited due to signal %d\n", WTERMSIG(wstatus));
  }

  if (WCOREDUMP(wstatus)) {
    printf("[launch-prog] child produced core dump\n");
  }

  return 0;
}
