#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>

static char child_stack[1024*1024];

static void
proc_ns(char *msg, pid_t pid)
{
  ssize_t buf_s;
  char buf[1024];
  char nspath[100] = {0};

  printf("%s\n", msg);

  if (sprintf(nspath, "/proc/%d/ns/mnt", pid) < 0) {
    printf("snprintf error occured for %d pid\n", pid);
    exit(1);
  }

  if ((buf_s = readlink(nspath, buf, 1024)) < 0) {
    printf("readlink failed with nspath %s\n", nspath);
    perror("readlink");
    exit(1);
  }

  buf[buf_s + 1] = '\0';

  printf("%d proc namespace is %s\n", pid, buf);
}

static int
childFunc(void *arg)
{
  printf("Process ID in child = %ld\n", (long) getpid());
  proc_ns("childFunc", getpid());
}

int
main(int argc, char *argv[])
{
  pid_t child_pid;

  if ((child_pid = clone(childFunc, child_stack + (1024*1024), CLONE_NEWPID | SIGCHLD, NULL) < 0)) {
    perror("clone");
    exit(1);
  }

  proc_ns("main proc", getpid());

  printf("PID of cloned process: %ld\n", (long) child_pid);
  waitpid(child_pid, NULL, 0);
  exit(EXIT_SUCCESS);
}
