#include "lib.h"

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/* random value provided OPEN_MAX is not defined */
#define OPEN_MAX_GUESS 256

long open_max(void);

int
main(void)
{
  long fds_limit;
  if ((fds_limit = open_max()) < 0) {
    printf("failed to define file descriptors limit\n");
    exit(1);
  }
  printf("fds limit is %ld\n", fds_limit);
  exit(0);
}

/*
 * Returns OPEN_MAX constant
 * that signes max file descriptor
 * (or amount of open files)
 * available to the process.
 * OPEN_MAX may be not defined
 * in headers file (<limits.h>)
 * and even during runtime!
 * PATH_MAX is this sort of constant too
 *
 * Though XSI-compliant systems
 * offer get/setrlimit syscalls
 * that change those limits on runtime
 **/
long
open_max(void)
{
  if (openmax == 0) { /* called the first time */
    errno = 0;
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
      if (errno == 0) {
        openmax = OPEN_MAX_GUESS;
      } else {
        printf("failed to call sysconf with _SC_OPEN_MAX\n");
        return -1;
      }
    }
  }

  return openmax;
}
