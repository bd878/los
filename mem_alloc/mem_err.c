/*
 * mem_err.c - causes process to fail
 * due to insufficient memory.
 * Memory is constrained via setrlimit
 */
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void print_lim(char *, rlim_t);

int main()
{
  long p_size;
  size_t alloc_size;
  void* block_addr;
  struct rlimit lim;

#ifdef _SC_PAGESIZE
  if ((p_size = sysconf(_SC_PAGESIZE)) < 0) {
#elif _SC_PAGE_SIZE
  if ((p_size = sysconf(_SC_PAGE_SIZE)) < 0) {
#endif
    perror("sysconf");
    exit(EXIT_FAILURE);
  }

  printf("pagesize: %d\n", p_size);

  lim.rlim_cur = p_size;
  lim.rlim_max = p_size;
  alloc_size = p_size*300;

  if (setrlimit(RLIMIT_AS, &lim) < 0) {
    perror("setrlimit");
    exit(EXIT_FAILURE);
  }

  if (getrlimit(RLIMIT_AS, &lim) < 0) {
    perror("getrlimit");
    exit(EXIT_FAILURE);
  }

  printf("new limits\n");
  print_lim("soft memory", lim.rlim_cur);
  print_lim("hard memory", lim.rlim_max);


  if ((block_addr = malloc(alloc_size)) == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  printf("somehow allocated %d size memory by addr: %d\n", alloc_size, block_addr);

  exit(EXIT_SUCCESS);
}

void print_lim(char *name, rlim_t lim)
{
  if (lim == RLIM_INFINITY) {
    printf("%s limit: infinity\n", name);
  } else {
    printf("%s limit: %d\n", name, lim);
  }
}
