/**
 * create_thread.c - creates POSIX thread
 */
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void child_thread(void *);

int main() {
  int s;
  pthread_t tid_self;
  pthread_t tid;

  tid_self = pthread_self();
  printf("[%d] self thread id: %u (0x%x)\n", getpid(),
    (unsigned int)tid_self, (unsigned int)tid_self);

  if ((s = pthread_create(&tid, NULL, &child_thread, NULL)) < 0) {
    errno = s;
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  pthread_join(tid);

  exit(EXIT_SUCCESS);
}

void child_thread(void *data);
