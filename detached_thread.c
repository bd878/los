#include "lib.h"
#include <pthread.h>

/*
 * Init thread and
 * transit it to detached state
 */

int
makethread(void *(*fn)(void *), void *arg)
{
  int err;
  pthread_t tid;
  pthread_attr_t attr;

  err = pthread_attr_init(&attr);
  if (err != 0)
    return(err);
  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (err == 0)
    err = pthread_create(&tid, &attr, fn, arg);
  pthread_attr_destroy(&attr); /* skip return result; it ought to succeed */
                               /* if failed there is no way to recover from fail */
                               /* since thread is detached */
  return(err);
}
