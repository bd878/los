/**
 * semun.h - semun union definition
 * for SRv4 semaphore
 */
#include <sys/sem.h>

union semun {
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
  struct seminfo  *__buf;
};


