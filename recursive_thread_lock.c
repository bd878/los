#include "lib.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

extern int makethread(void *(*)(void *), void *);

/* TODO: to_info considered incomplete */
/* do sth with it later */
struct to_into {
  void (*to_fn)(void *);   /* thread func */
  void *to_arg;            /* args */
  struct timespec to_wait; /* start time */
};

#define SECTONSEC 10000000000 /* nanoseconds per second */

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM) nanosleep((REQ, (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000 /* microseconds in second */

/* own nanosleep implementation */
void
clock_gettime(int id, struct timespec *tsp)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  tsp->tv_sec = tv.tv_sec;
  tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

void *
timeout_helper(void *arg)
{
  struct to_info *tip;

  tip = (struct to_info *)arg;
  clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);
  (*tip->to_fn)(tip->to_arg);
  free(arg);

  return(0);
}

void
timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
  struct timespec now;
  struct to_info *tip;
  int err;

  clock_gettime(CLOCK_REALTIME, &now);
  if ((when->tv_sec > now.tv_sec) ||
      (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
    tip = malloc(sizeof(struct to_info));
    if (tip != NULL) {
      tip->to_fn = func;
      tip->to_arg = arg;
      tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
      if (when->tv_nsec >= now.tv_nsec) {
        tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
      } else {
        tip->to_wait.tv_sec--;
        tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
      }

      err = makethread(timeout_helper, (void *)tip); /* detached_thread file */
      if (err == 0)
        return;
      else
        free(tip);
    }
  }

  /*
   * Reaches this point if when <= now,
   * || malloc failes,
   * || cannot create new thread,
   * so we just calls the func
   **/
  (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void
retry(void *arg)
{
  pthread_mutex_lock(&mutex);

  /* do func actions ... */

  pthread_mutex_unlock(&mutex);
}

int
main(void)
{
  int err, condition, arg;
  struct timespec when;

  if ((err = pthread_mutexattr_init(&attr)) != 0)
    err_exit(err, "failed to call pthread_mutexattr_init");
  if ((err = pthread_mutexattr_settype(&attr,
    PTHREAD_MUTEX_RECURSIVE)) != 0)
    err_exit(err, "cannot set recursive mutex type");
  if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
    err_exit(err, "cannot set recursive mutex");

  /* continue processing ... */

  pthread_mutex_lock(&mutex);

  /*
   * Ensure that state variable is under protection,
   * provide atomicity for checking and timeout func call
   **/
  if (condition) {
    /* compute start time "when" func */
    clock_gettime(CLOCK_REALTIME, &when);
    when.tv_sec += 10; /* in 10 seconds till current time */
    timeout(&when, retry, (void *)((unsigned long)arg));
  }

  /* continue processing ... */

  pthread_mutex_unlock(&mutex);

  exit(0);
}
