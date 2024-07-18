/*
 * get_shmem.c - obtain anonymous shared memory
 */
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int sh_id;
  long page_size;
  struct shmid_ds buf;

  if ((page_size = sysconf(_SC_PAGE_SIZE)) < 0) {
    perror("sysconf");
    exit(EXIT_FAILURE);
  }

  printf("page size: %d\n", page_size);

  if ((sh_id = shmget(IPC_PRIVATE, page_size*2, IPC_CREAT | IPC_EXCL)) < 0) {
    perror("smget");
    exit(EXIT_FAILURE);
  }

  printf("shmget id: %d\n", sh_id); 

  if (shmctl(sh_id, IPC_STAT, &buf) < 0) {
    perror("shmctl");
    exit(EXIT_FAILURE);
  }

  printf("me: %d\n", getpid());

  printf("size: %d\n", buf.shm_segsz);
  printf("current attaches: %d\n", buf.shm_nattch);
  printf("creator pid: %d\n", buf.shm_cpid);
  printf("created at: %d\n", buf.shm_ctime);

  exit(EXIT_SUCCESS);
}
