#include "lib.h"
#include <pthread.h>

pthread_t ntid;

void printids(const char *);
void *thr_fn();

int
main(void)
{
  int err;

  err = pthread_create(&ntid, NULL, thr_fn, NULL);
  if (err != 0)
    err_exit(err, "cannot create new thread");
  printids("main thread:");
  sleep(1);
  exit(0);
}

void
printids(const char *s)
{
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();
  /* pthread_t might be a struct */
  /* if we output it, an address would output. */
  /* Process id of these threads is the same */
  printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
    (unsigned int)tid, (unsigned int)tid);
}

void *
thr_fn()
{
  printids("new thread: ");
  return((void *)0);
}
