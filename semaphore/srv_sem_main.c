/**
 * srv_sem_main.c - srv_sem.c driver example
 */
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#include "srv_sem.h"

int main() {
  int semid;

  if ((semid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  init_sem(semid);

  print_sem_stat(semid);

  wait_sighup();

  printf("exit.\n");

  cleanup_sem(semid);

  return 0;
}


