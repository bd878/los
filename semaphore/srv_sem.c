/**
 * new_sem.c - create new semaphore set,
 * init with 1 value and cleanup on exit
 */
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>

#include "srv_sem.h"
#include "semun.h"

void cleanup_sem(int semid)
{
  int e = errno;
  semctl(semid, 0, IPC_RMID);
  if (errno > 0)
    perror("semctl");
  e = errno;
}

void init_sem(int semid)
{
  struct sembuf sb;

  sb.sem_num = 0;
  sb.sem_op = 1;
  sb.sem_flg = SEM_UNDO;

  if (semop(semid, &sb, 1) < 0) {
    cleanup_sem(semid);
    exit(EXIT_FAILURE);
  }
}

void wait_sighup()
{
  struct sigaction sigset;

  sigset.sa_handler = &sighup_handler;
  sigemptyset(&sigset.sa_mask);
  if (sigaction(SIGHUP, &sigset, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  printf("[%d] waiting for SIGHUP...\n", getpid());
  pause();
}

void sighup_handler(int)
{
  printf("[%d] SIGHUP\n", getpid());
}

void print_sem_stat(int semid)
{
  union semun semstat;
  struct semid_ds buf;

  semstat.buf = &buf;

  if (semctl(semid, 0, IPC_STAT, semstat) < 0) {
    cleanup_sem(semid);
    exit(EXIT_FAILURE);
  }

  printf("=== semid=%d ===\n", semid);
  printf("sizeof(*semstat.buf): %d\n", sizeof(*(semstat.buf)));
  printf("creation time: %s", ctime((time_t *)&(semstat.buf->sem_ctime)));
  printf("last access time: %s", ctime((time_t *)&(semstat.buf->sem_otime)));
  printf("number of sems in a set: %u\n", semstat.buf->sem_nsems);
  printf("\n");
}
