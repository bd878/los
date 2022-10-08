#include <pthread.h>

/*
 * Demonstrates condition variables
 * usecase. It is a tool to sync
 * threads alongside with mutexes
 **/

struct msg {
  struct msg *m_next;
  /* ... other fields ... */
};

struct msg *workq;

/* keeps messages queue state */
pthread_cond_t qready = PTHREAD_COND_INITIALIZER; /* protected by qlock */

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void
process_msg(void)
{
  struct msg *mp;

  for (;;) {
    pthread_mutex_lock(&qlock);
    while (workq == NULL)
      pthread_cond_wait(&qready, &qlock);
    mp = workq;
    workq = mp->m_next;
    pthread_mutex_unlock(&qlock);
    /* handle mp event message */
  }
}

void
enqueue_msg(struct msg *mp)
{
  pthread_mutex_lock(&qlock);
  mp->m_next = workq;
  workq = mp;
  pthread_mutex_unlock(&qlock); /* unlock before cond_signal */
  pthread_cond_signal(&qready); /* race is possible, but till now it is ok */
}

int
main()
{
}
