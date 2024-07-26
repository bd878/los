/**
 * write_shmem.c - starts two non-sibling processes,
 * attaches shared memory segment in each
 * and updates a value between them.
 */
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "detach_process.h"
#include "sync_pipe.h"

void
detached_proc(int *);

int main()
{
  pid_t me, cid;
  int shmid;
  long psize;
  int fd[2];

  psize = getpagesize();

  me = getpid();
  printf("me: %d\n", me);

  OPEN_PIPE(fd);

  detach(fd, &detached_proc); /* forks twice */

  wait_child(fd, &cid); 
  printf("[%d] detached process: %d\n", me, cid);

  /* call after fork (detach)
   * because child process inherits attached shared
   * memory segments */
  if ((shmid = shmget(IPC_PRIVATE, psize*2, IPC_CREAT|IPC_EXCL)) < 0) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  printf("[%d] parent shmid: %d\n", me, shmid);

  exit(EXIT_SUCCESS);
}

void
detached_proc(int fd[2])
{
  pid_t me;

  me = getpid();

  printf("[%d] child\n", me);
  return;
}
