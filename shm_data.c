#include "lib.h"
#include <sys/shm.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600 /* rw owner */

char array[ARRAY_SIZE];

int
main(void)
{
  int shmid;
  char *ptr, *shmptr;

  printf("array[] from %p to %p\n", (void *)&array[0], (void *)&array[ARRAY_SIZE]);
  printf("stack is approximately at %p\n", (void *)&shmid);

  if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
    printf("failed to call malloc\n");
    exit(1);
  }

  printf("heap is from %p to %p\n",
    (void *)ptr, (void *)ptr+MALLOC_SIZE);

  if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
    printf("failed to call shmget\n");
    exit(1);
  }

  if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1) {
    printf("failed to call shmat\n");
    exit(1);
  }

  printf("shared memory is from %p to %p\n",
    (void *)shmptr, (void *)shmptr+SHM_SIZE);

  if (shmctl(shmid, IPC_RMID, 0) < 0) {
    printf("failed to call shmctl\n");
    exit(1);
  }

  exit(0);
}
