#include "lib.h"
#include <pthread.h>

void *thr_fn1();
void *thr_fn2();

int
main(void)
{
  int err;
  pthread_t tid1, tid2;
  void *tret;

  err = pthread_create(&tid1, NULL, thr_fn1, NULL);
  if (err != 0)
    err_exit(err, "cannot create thread 1");
  err = pthread_create(&tid2, NULL, thr_fn2, NULL);
  if (err != 0)
    err_exit(err, "cannot create thread 2");
  err = pthread_join(tid1, &tret);
  if (err != 0)
    err_exit(err, "cannot join thread 1");
  printf("exit id thread 1: %ld\n", (long)tret);
  err = pthread_join(tid2, &tret);
  if (err != 0)
    err_exit(err, "cannot join thread 2");
  printf("exit id thread 2: %ld\n", (long)tret);
  exit(0);
}

void *
thr_fn1()
{
  printf("thread 1: exit\n");
  return((void *)1);
}

void *
thr_fn2()
{
  printf("thread 2: exit\n");
  pthread_exit((void *)2);
}
