/**
 * sem_proc.c - init semaphore and use it
 * between multiple processes
 */
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../shmem/sync_pipe.h"
#include "srv_sem.h"
#include "semun.h"

int main() {
  pid_t cid;
  pid_t me;
  int semid;
  int fd[2];
  struct sembuf bf;

  me = getpid();
  
  if ((semid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  OPEN_PIPE(fd);

  init_sem(semid);

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid == 0) { /* child */
    pid_t me;
    close(fd[1]); /* write end */

    me = getpid();
    WAIT_PARENT(fd);

    bf.sem_num = 0;
    bf.sem_op = -1; /* decrement by this value */
    bf.sem_flg = SEM_UNDO;

    if (semop(semid, &bf, 1) < 0) {
      cleanup_sem(semid);
      exit(EXIT_FAILURE);
    }

    printf("[%d] child second\n", me);

    exit(EXIT_SUCCESS);
  }

  close(fd[0]); /* read end */

  bf.sem_num = 0;
  bf.sem_op = -1;
  bf.sem_flg = SEM_UNDO;

  if (semop(semid, &bf, 1) < 0) {
    cleanup_sem(semid);
    exit(EXIT_FAILURE);
  }

  TELL_CHILD(fd);
  printf("[%d] parent first\n", me);

  print_sem_stat(semid);

  printf("[%d] unblock child\n", me);

  wait_sighup();

  bf.sem_num = 0;
  bf.sem_op = 1; /* increment semaphore by this value */
  if (semop(semid, &bf, 1) < 0) {
    cleanup_sem(semid);
    exit(EXIT_FAILURE);
  }

  printf("[%d] waiting for child to exit: %d\n", me, cid);
  waitpid(cid, NULL, 0);
  printf("[%d] child exited: %d\n", me, cid);

  if (semctl(semid, 0, IPC_RMID)) {
    perror("semctl");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
