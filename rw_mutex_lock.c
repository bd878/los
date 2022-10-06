#include <stdlib.h>
#include <pthread.h>

struct job {
  struct job *j_next;
  struct job *j_prev;
  pthread_t j_id; /* executing report id */
  /* ... other fields ... */
};

struct queue {
  struct job *q_head;
  struct job *q_tail;
  pthread_rwlock_t q_lock;
};

/* init the queue */
int
queue_init(struct queue *qp)
{
  int err;

  qp->q_head = NULL;
  qp->q_tail = NULL;
  err = pthread_rwlock_init(&qp->q_lock, NULL);
  if (err != 0)
    return(err);
  /* ... further initialization ... */
  return(0);
}

/* add a job at the beginning */
void
job_insert(struct queue *qp, struct job *jp)
{
  pthread_rwlock_wrlock(&qp->q_lock);
  jp->j_next = NULL;
  jp->j_prev = qp->q_tail;
  if (qp->q_tail != NULL)
    qp->q_tail->j_next = jp;
  else
    qp->q_head = jp; /* queue is empty */
  qp->q_tail = jp;
  pthread_rwlock_unlock(&qp->q_lock);
}

/* removes job from the queue */
void
job_remove(struct queue *qp, struct job *jp)
{
  pthread_rwlock_wrlock(&qp->q_lock);
  if (jp == qp->q_head) {
    qp->q_head = jp->j_next;
    if (qp->q_tail == jp)
      qp->q_tail = NULL;
    else
      jp->j_next->j_prev = jp->j_prev;
  } else if (jp == qp->q_tail) {
    qp->q_tail = jp->j_prev;
    jp->j_prev->j_next = jp->j_next;
  } else {
    jp->j_prev->j_next = jp->j_next;
    jp->j_next->j_prev = jp->j_prev;
  }
  pthread_rwlock_unlock(&qp->q_lock);
}

/* finds a job with specified id */
struct job *
job_find(struct queue *qp, pthread_t id)
{
  struct job *jp;

  if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
    return(NULL);

  for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
    if (pthread_equal(jp->j_id, id))
      break;

  pthread_rwlock_unlock(&qp->q_lock);
  return(jp);
}

int
main()
{
}
