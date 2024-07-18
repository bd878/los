/*
 * data_seg_end.c - prints the address
 * past the end of current program's data segment
 */
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv)
{
  void* start_addr;
  void* brk_addr;
  void* brk_addr2;
  void* out_limit_addr;
  long page_size;
  long ds_size;
  char* me;
  struct rlimit lim, lim_get;

  if ((page_size = sysconf(_SC_PAGE_SIZE)) < 0) {
    perror("sysconf");
    exit(EXIT_FAILURE);
  }

  printf("page size: %d\n", page_size);
  if (page_size == 0) {
    fprintf(stderr, "page size is 0\n");
    exit(EXIT_FAILURE);
  }

  me = argv[0];

  if ((start_addr = malloc(0)) == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  printf("start addr: %d\n", start_addr);

  if ((brk_addr = sbrk(0)) < 0) {
    perror("sbrk");
    exit(EXIT_FAILURE);
  }

  printf("brk addr: %d\n", brk_addr);

  ds_size = brk_addr - start_addr;
  printf("data segment size: %d\n", ds_size); 
  printf("pages: %d\n", ds_size / page_size);

  lim.rlim_cur = ds_size+1;
  lim.rlim_max = ds_size+2;
  if (setrlimit(RLIMIT_DATA, &lim) < 0) {
    perror("setrlimit");
    exit(EXIT_FAILURE);
  }

  if (getrlimit(RLIMIT_DATA, &lim_get) < 0) {
    perror("getrlimit");
    exit(EXIT_FAILURE);
  }

  printf("===\n");
  printf("new limits:\n");
  printf("soft: %d\n", lim_get.rlim_cur);
  printf("hard: %d\n", lim_get.rlim_max);

  if ((brk_addr2 = sbrk(0)) < 0) {
    perror("sbrk");
    exit(EXIT_FAILURE);
  }

  if ((out_limit_addr = malloc(lim_get.rlim_cur)) < 0) {
    perror("malloc");
    exit(EXIT_FAILURE);
  } else if (out_limit_addr == NULL) {
    // should fail
    fprintf(stderr, "[%s] WARNING: malloc errno: %s\n", me, strerror(errno));
  }

  printf("out limit addr: %d\n", out_limit_addr);

  printf("brk addr: %d\n", brk_addr2);

  exit(EXIT_SUCCESS);
}
