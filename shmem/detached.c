/**
 * detached.c - detached proc to read/write
 * from shared data
 */

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "detached.h"

void detached_proc(int *fd) {
  pid_t me;
  int shmid, semid;
  char *addr;
  struct sembuf buf;

  me = getpid();

  // printf("[%d] child second\n", me);

  if ((shmid = get_or_create_shmem(0)) < 0) {
    fprintf(stderr, "[%d] failed to obtain shared memory: %s\n", me, strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("[%d] child shmid: %d\n", me, shmid);

  if ((semid = get_or_create_sem(0)) < 0) {
    fprintf(stderr, "[%d] failed to obtain semaphore: %s\n", me, strerror(errno));
    exit(EXIT_FAILURE);
  }

  buf.sem_num = 0;
  buf.sem_op = 0;
  if (semop(semid, &buf, 1) < 0) {
    fprintf(stderr, "[%d] failed to wait on semaphore: %s\n", me, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((addr = shmat(shmid, NULL, 0)) < 0) {
    perror("shmat");
    return;
  }

  printf("[%d] child received a string from shared memory: %s\n",
    me, addr);

  shmdt(addr); /* detach shared memory */

  return;
}

int get_or_create_sem(int flags) {
  key_t key;
  int semid;

  if ((key = ftok("write_shmem.c", 2)) < 0) {
    perror("ftok");
    return -1;
  }

  if ((semid = semget(key, 1, flags)) < 0) {
    perror("semget");
    return -1;
  }

  return semid;
}

int get_or_create_shmem(int flags) {
  long psize;
  key_t key;
  int shmid;

  psize = getpagesize();

  if ((key = ftok("write_shmem.c", 1)) < 0) {
    perror("ftok");
    return -1;
  }

  if ((shmid = shmget(key, psize*2, flags)) < 0) {
    perror("shmget");
    return -1;
  }

  return shmid;
}

void rm_shmem(int shmid)
{
  int e = errno;
  shmctl(shmid, IPC_RMID, 0);
  if (errno != 0)
    perror("shmctl");
  errno = e;
}

void rm_sem(int semid) {
  int e = errno;
  semctl(semid, IPC_RMID, 0);
  if (errno != 0)
    perror("semctl");
  errno = e;
}
