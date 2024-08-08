/**
 * set_affinity.c - attach current process to specified CPU,
 * detach all other threads
 */
#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void print_affinity();
void set_affinity(int);

int main() {
  print_affinity();
  set_affinity(2);
  print_affinity();

  exit(EXIT_SUCCESS);
}

void print_affinity() {
  cpu_set_t set;

  CPU_ZERO(&set);
  errno = 0;
  if (sched_getaffinity(0, sizeof(cpu_set_t), &set) < 0) {
    if (errno != 0) {
      perror("sched_getaffinity");
      exit(EXIT_FAILURE);
    }
  }
  printf("[%d] number of cpus in set: %d\n", getpid(), CPU_COUNT(&set));
}

void set_affinity(int cpu) {
  cpu_set_t *set;

  set = CPU_ALLOC(cpu);
  CPU_SET(cpu, set);

  printf("[%d] set cpu affinity to %d\n", getpid(), cpu);
  if (sched_setaffinity(0, sizeof(cpu_set_t), set) < 0) {
    perror("sched_setaffinity");
    exit(EXIT_FAILURE);
  }
}
