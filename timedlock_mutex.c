#include "lib.h"
#include <pthread.h>

/*
 * Tries to repeat mutex lock
 * to let pthread_mutex_timedlock
 * exit by specified timeout
 **/
int
main(void)
{
  int err;
  struct timespec tout;
  struct tm *tmp;
  char buf[64];

  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&lock);
  printf("mutex is locked\n");

  clock_gettime(CLOCK_REALTIME, &tout);
  tmp = localtime(&tout.tv_sec);
  strftime(buf, sizeof(buf), "%r", tmp);

  printf("current time: %s\n", buf);
  tout.tv_sec += 10; /* 10 seconds from current time */

  err = pthread_mutex_timedlock(&lock, &tout); /* attention: may cause a deadlock */
  clock_gettime(CLOCK_REALTIME, &tout);
  tmp = localtime(&tout.tv_sec);
  strftime(buf, sizeof(buf), "%r", tmp);
  printf("current time: %s\n", buf);

  if (err == 0)
    printf("mutex is locked again!\n");
  else
    printf("failed to lock mutex: %s\n", strerror(err));
  exit(0);
}
