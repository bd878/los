#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>

static char child_stack[1024*1024];

static int
childFunc(void *arg)
{
  printf("Process ID in child = %ld\n", (long) getpid());
  return 0;
}

int
main(int argc, char *argv[])
{
  pid_t child_pid;

  if ((child_pid = clone(childFunc, child_stack + (1024*1024), CLONE_NEWPID | SIGCHLD, NULL) < 0)) {
    perror("clone");
    exit(1);
  }

  printf("PID of cloned process: %ld\n", (long) child_pid);
  waitpid(child_pid, NULL, 0);
  exit(EXIT_SUCCESS);
}
