#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;
/* initializer for statically allocated mutex only */
/* for dynamic allocation use pthread_mutex_init */

struct foo {
  int f_count;
  pthread_mutex_t f_lock;
  int f_id;
  struct foo *f_next; /* guarded via hashlock mutex */
  /* ... other struct fields ... */
};

struct foo *
foo_alloc(int id) /* dynamically creates an object */
{
  struct foo *fp;
  int idx;

  if ((fp = malloc(sizeof(struct foo))) != NULL) {
    fp->f_count = 1;
    fp->f_id = id;
    if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
      free(fp);
      return(NULL);
    }
    idx = HASH(fp);
    pthread_mutex_lock(&hashlock);
    fp->f_next = fh[idx];
    fh[idx] = fp->f_next;
    pthread_mutex_lock(&fp->f_lock);
    pthread_mutex_unlock(&hashlock);
    /* ... continue initialization ... */
    pthread_mutex_unlock(&fp->f_lock);
  }
  return(fp);
}

void
foo_hold(struct foo *fp) /* increase pointer */
{
  pthread_mutex_lock(&fp->f_lock);
  fp->f_count++;
  pthread_mutex_unlock(&fp->f_lock);
}

struct foo *
foo_find(int id) /* find existing object to check if it is possible to release memory */
{
  struct foo *fp;

  pthread_mutex_lock(&hashlock);
  for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
    if (fp->f_id == id) {
      foo_hold(fp);
      break;
    }
  }
  pthread_mutex_unlock(&hashlock);
  return(fp);
}

void
foo_rele(struct foo *fp) /* decrease pointer */
{
  struct foo *tfp;
  int idx;

  pthread_mutex_lock(&fp->f_lock);
  if (fp->f_count == 1) { /* last grasp */
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_lock(&hashlock);
    pthread_mutex_lock(&fp->f_lock);
    /* check repeat */
    if (fp->f_count != 1) {
      fp->f_count--;
      pthread_mutex_unlock(&fp->f_lock);
      pthread_mutex_unlock(&hashlock);
      return;
    }
    /* remove from list */
    idx = HASH(fp->f_id);
    tfp = fh[idx];
    if (tfp == fp) {
      fh[idx] = fp->f_next;
    } else {
      while (tfp->f_next != fp) {
        tfp = tfp->f_next;
      }
      tfp->f_next = fp->f_next;
    }
    pthread_mutex_unlock(&hashlock);
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_destroy(&fp->f_lock);
    free(fp);
  } else {
    fp->f_count--;
    pthread_mutex_unlock(&fp->f_lock);
  }
}

int
main()
{
}
