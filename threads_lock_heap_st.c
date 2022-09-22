#include <stdlib.h>
#include <pthread.h>

struct foo;

/*
 * This set of functions
 * allow to use object on heap
 * by several threads.
 * It uses counter,
 * and once reached 0,
 * frees memory under object
 **/

struct foo *foo_alloc(int); /* creates object */
void foo_hold(struct foo *); /* locks mutex for object use */
void foo_rele(struct foo *); /* unlocks mutex to free object (and unblock waiting threads) */

int
main()
{
  return(0);
}

struct foo {
  int f_count;
  pthread_mutex_t f_lock;
  int f_id;
  /* ... other fields ... */
};

struct foo *
foo_alloc(int id)
{
  struct foo *fp;

  if ((fp = malloc(sizeof(struct foo))) != NULL) {
    fp->f_count = 1;
    fp->f_id = id;
    /* pthread_mutex_init is for dynamic mutex allocation */
    /* static mutexes init with constant PTHREAD_MUTEX_INITIALIZER */
    if (pthread_mutex_init(&fp->f_lock, NULL) != 0) { /* NULL for default mutex values */
      free(fp);
      return(NULL);
    }
    /* continue initialization */
  }
  return(fp);
}

/* does not support multithreading */
/* since one thread may free memory */
/* while other thread would be blocked by foo_hold */
/* and then would try to mutate freed resource */
void
foo_hold(struct foo *fp)
{
  pthread_mutex_lock(&fp->f_lock); /* blocking operation */
                                   /* pthread_mutex_trylock is unblocking */
  fp->f_count++;
  pthread_mutex_unlock(&fp->f_lock);
}

void
foo_rele(struct foo *fp)
{
  pthread_mutex_lock(&fp->f_lock);
  /* --fp->f_count is side effect */
  if (--fp->f_count == 0) { /* last pointer */
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_destroy(&fp->f_lock);
    free(fp);
  } else {
    pthread_mutex_unlock(&fp->f_lock);
  }
}
