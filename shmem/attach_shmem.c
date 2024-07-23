/**
 * attach_shmem.c - attach shared memory segment
 */
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int shm_id;
  void* attached_addr;
  long psize;
  struct shmid_ds buf;

  psize = getpagesize();

  if ((shm_id = shmget(IPC_PRIVATE, psize*2, IPC_CREAT|IPC_EXCL)) < 0) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  if ((attached_addr = shmat(shm_id, 0, 0)) < 0) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  if (shmctl(shm_id, IPC_STAT, &buf) < 0) {
    perror("shmctl");
    exit(EXIT_FAILURE);
  }

  printf("attached address: %d\n", attached_addr);
  printf("number of processes: %d\n", buf.shm_nattch);

  exit(EXIT_SUCCESS);
}
