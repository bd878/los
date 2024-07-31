/**
 * write_shmem.c - starts two non-sibling processes,
 * attaches shared memory segment in each
 * and updates a value between them.
 */
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
 
#include "detach_process.h"
#include "detached.h"
#include "sync_pipe.h"
 
static const char str[] = "hello, memory!\0";
 
int main()
 {
  pid_t me, cid;
  int shmid, semid;
  int fd[2];
  char *addr;
  size_t len;
  struct sembuf buf;
 
  me = getpid();
  printf("me: %d\n", me);
  len = strlen(str) + 1;
 
  OPEN_PIPE(fd);
 
  cid = detach(fd, &detached_proc); /* forks twice */
 
  /* call after fork (detach)
   * because child process inherits attached shared
   * memory segments */
  if ((shmid = get_or_create_shmem(IPC_CREAT|IPC_EXCL|0666)) < 0) {
    fprintf(stderr, "[%d] failed to create shared memory: %s\n", me, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((semid = get_or_create_sem(IPC_CREAT|IPC_EXCL|0666)) < 0) {
    rm_shmem(shmid);
    fprintf(stderr, "[%d] failed to create semaphor: %s\n", me, strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* set semaphore to 1 */
  /* child expects 0 to continue */
  buf.sem_num = 0;
  buf.sem_op = 1;
  if (semop(semid, &buf, 1) < 0) {
    rm_shmem(shmid);
    rm_sem(semid);
    perror("semop");
    exit(EXIT_FAILURE);
  }

  if ((addr = shmat(shmid, NULL, 0)) < 0) {
    rm_shmem(shmid);
    rm_sem(semid);
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  memcpy(addr, str, len);

  // tell child to read from shmem
  // printf("[%d] parent first\n", me);
  if (TELL_CHILD(fd) < 0) {
    shmdt(addr);
    rm_shmem(shmid);
    rm_sem(semid);
    exit(EXIT_FAILURE);
  }  

  // semval is 1 so far, decrement it and resume a child
  buf.sem_num = 0;
  buf.sem_op = -1;
  if (semop(semid, &buf, 1) < 0) {
    shmdt(addr);
    rm_shmem(shmid);
    rm_sem(semid);
    perror("semop");
    exit(EXIT_FAILURE);
  }

  // wait child to exit
  if (wait_child(cid) < 0) {
    shmdt(addr);
    rm_shmem(shmid);
    rm_sem(semid);
    exit(EXIT_FAILURE);
  }
 
  printf("[%d] detached process: %d\n", me, cid);
  printf("[%d] parent shmid: %d\n", me, shmid);
 
  shmdt(addr); /* detach shared memory */
  rm_shmem(shmid);
  rm_sem(semid);

  exit(EXIT_SUCCESS);
}
 
