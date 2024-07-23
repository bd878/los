/**
 * fork_change_stack.c - what happens if change a value
 * by a pointer, located on a parent's stack
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  pid_t me, cid;
  int* val;

  val = malloc(sizeof(int));
  if (errno > 0) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  me = getpid();

  printf("[%d] val before init: %d, addr: %d\n", me, *val, val);
  *val = 10;

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid == 0) { /* child */
    pid_t me;
    me = getpid();
    *val = 11;
    printf("[%d] val: %d, addr: %d\n", me, *val, val);
    exit(EXIT_SUCCESS);
  }

  printf("[%d] cid: %d\n", me, cid);

  if (waitpid(cid, NULL, 0) < 0) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }

  printf("[%d] val: %d, addr: %d\n", me, *val, val);
  exit(EXIT_SUCCESS);
}
