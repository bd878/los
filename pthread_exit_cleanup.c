#include "lib.h"
#include <pthread.h>

/*
 * POSIX threads allow
 * to register handlers
 * that will trigger on
 * thread exit.
 * Demonstarte use of such
 * methods (pthread_cleanup_push and
 * pthread_cleanup_pop)
 **/

void cleanup(void *arg);
void *thr_fn1();
void *thr_fn2();

int
main(void)
{
  int err;
  pthread_t tid1, tid2;
  void *tret;

  err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
  if (err != 0)
    err_exit(err, "cannot create thread 1");
  err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
  if (err != 0)
    err_exit(err, "cannot create thread 2");
  err = pthread_join(tid1, &tret);
  if (err != 0)
    err_exit(err, "cannot attach thread 1");
  printf("thread 1 return value: %ld\n", (long)tret);
  err = pthread_join(tid2, &tret);
  if (err != 0)
    err_exit(err, "cannot attach thread 2");
  printf("thread 2 return value: %ld\n", (long)tret);
  exit(0);
}

void
cleanup(void *arg)
{
  printf("exit: %s\n", (char *)arg);
}

void *
thr_fn1(void *arg)
{
  printf("thread 1 is launching\n");
  pthread_cleanup_push(cleanup, "thread 1, first handler");
  pthread_cleanup_push(cleanup, "thread 1, second handler");
  printf("thread 1, handlers registration is over\n");
  if (arg)
    return((void *)1); /* push/pop must be paired */
                       /* exit in the middle of push/pop */
                       /* may lead to panic exit on runtime */
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
  return((void *)1);
}

void *
thr_fn2(void *arg)
{
  printf("thread 2 is launching\n");
  pthread_cleanup_push(cleanup, "thread 2, first handler");
  pthread_cleanup_push(cleanup, "thread 2, second handler");
  printf("thread 2, handlers registration is over\n");
  if (arg)
    pthread_exit((void *)2);
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
  pthread_exit((void *)2);
}
