#include "lib.h"
#include <pthread.h>

/* 
 * Display result value
 * from threads
 * and misuse of result value
 * allocated on func stack (in thread)
 **/

struct foo; 

void printfoo(const char *, const struct foo *);
void *thr_fn1();
void *thr_fn2();

int
main(void)
{
  int err;
  pthread_t tid1, tid2;
  struct foo *fp;

  err = pthread_create(&tid1, NULL, thr_fn1, NULL); /* first thread */
  if (err != 0) {
    err_exit(err, "cannot create thread 1");
  }
  err = pthread_join(tid1, (void *)&fp); /* receive fp struct from first thread */
                                         /* free 1 thread resources */
  if (err != 0)
    err_exit(err, "failed to join thread 1");
  sleep(1);
  printf("parent process creating second thread\n");
  err = pthread_create(&tid2, NULL, thr_fn2, NULL); /* second thread */
                                                    /* not interested in result value from 2 thread */
  if (err != 0) {
    err_exit(err, "failed to create thread 2");
  }
  sleep(1);
  printfoo("parent process:\n", fp); /* output struct from 1 thread */
  exit(0);
}

struct foo {
  int a, b, c, d;
};

void
printfoo(const char *s, const struct foo *fp)
{
  printf("%s", s);
  printf(" struct on address 0x%lx\n", (unsigned long)fp);
  printf(" foo.a = %d\n", fp->a);
  printf(" foo.b = %d\n", fp->b);
  printf(" foo.c = %d\n", fp->c);
  printf(" foo.d = %d\n", fp->d);
}

void *
thr_fn1()
{
  struct foo foo = {1, 2, 3, 4}; /* struct on stack */

  printfoo("thread 1:\n", &foo);
  pthread_exit((void *)&foo); /* rval_ptr receives pointer to static variable */
                              /* potentially damagable */
}

void *
thr_fn2()
{
  printf("thread 2: id = %lu\n", (unsigned long)pthread_self());
  pthread_exit((void *)0);
}
