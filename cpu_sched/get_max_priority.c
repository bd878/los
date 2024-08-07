/**
 * sched_get_priority.c - returns this cpu thread's scheduler
 * policy, gets scheduler priority
 */
#include <sched.h>
#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_priority_ranges(int);

int main() {
  int policy;
  struct sched_param param;

  if (sched_getparam(0, &param) < 0) {
    perror("sched_getparam");
    exit(EXIT_FAILURE);
  }

  if ((policy = sched_getscheduler(getpid())) < 0) {
    perror("sched_getscheduler");
    exit(EXIT_FAILURE);
  }
  printf("[%d] current thread's policy: %d\n", getpid(), policy);

  printf("[%d] scheduler policy is: ", getpid());
  switch(policy) {
#ifdef SCHED_FIFO
    case SCHED_FIFO: {
      printf("SCHED_FIFIO\n");
      break;
    }
#endif

#ifdef SCHED_RR
    case SCHED_RR: {
      printf("SCHED_RR\n");
      break;
    }
#endif

#ifdef SCHED_OTHER
    case SCHED_OTHER: {
      printf("SCHED_OTHER\n");
      break;
    }
#endif

#ifdef SCHED_DEADLINE
    case SCHED_DEADLINE: {
      printf("SCHED_DEADLINE\n");
      break;
    }
#endif

#ifdef SCHED_IDLE
    case SCHED_IDLE: {
      printf("SCHED_IDLE\n");
      break;
    }
#endif

#ifdef SCHED_BATCH
    case SCHED_BATCH: {
      printf("SCHED_BATCH\n");
      break;
    }
#endif

#ifdef SCHED_FIFIO
    case SCHED_FIFO: {
      printf("SCHED_FIFO\n");
      break;
    }
#endif
    default:
      printf("<unknown policy>\n");
  }

  printf("\n");
  print_priority_ranges(policy);
  print_priority_ranges(SCHED_OTHER);
  print_priority_ranges(SCHED_RR);

  exit(EXIT_SUCCESS);
}

void print_priority_ranges(int policy) {
  int priority_min, priority, priority_max;

  printf("[%d] ranges for policy: %d\n", getpid(), policy);
  if ((priority_min = sched_get_priority_min(policy)) < 0) {
    perror("sched_get_priority_min");
    exit(EXIT_FAILURE);
  }
  if ((priority_max = sched_get_priority_max(policy)) < 0) {
    perror("sched_get_priority_max");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  if ((priority = getpriority(PRIO_PROCESS, 0)) < 0) {
    if (errno != 0) {
      fprintf(stderr, "[%d] failed to call getpriority: %s\n",
        getpid(), strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
  printf("[%d] priority min: %d\n", getpid(), priority_min);
  printf("[%d] priority: %d\n", getpid(), priority);
  printf("[%d] priority max: %d\n", getpid(), priority_max);
  printf("\n");
}
