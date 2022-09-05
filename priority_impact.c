#include "lib.h"
#include <errno.h>
#include <sys/time.h>

#if defined(MACOS)
#include <sys/syslimits.h>
#elif defined(SOLARIS)
#include <limits.h>
#elif defined(BSD)
#include <sys/param.h>
#endif

unsigned long long count;

struct timeval end;

void
checktime(char *str)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec) {
    printf("%s counter = %lld\n", str, count);
    exit(0);
  }
}

/*
 * Illustrate an impact of priority level.
 * Two processes gradually increase their
 *   priority counters.
 * Parent process runs with default nice level,
 *   child process changes coef accordingly
 *   to command line argument.
 * In 10 seconds both processes exit and print
 *   counters.
 * Comparing counters from processes of varying
 *   priority levels we might determine
 *   how nice level affects on processes planning.
 * Launch on single core. Multicore systems
 *   might execute subprocesses in parallel
 * (lookup man sched_setaffinity)
 **/
int
main(int argc, char *argv[])
{
  pid_t pid;
  char *s;
  int nzero, ret;
  int adj = 0;

  setbuf(stdout, NULL); /* turn stdio buffers off */
#if defined(NZERO)
  nzero = NZERO; /* NZERO is a system constant that
                    keeps default nice level in
                    this system */
#elif defined(_SC_NZERO)
  nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
  printf("NZERO = %d\n", nzero);
  if (argc == 2) {
    adj = strtol(argv[1], NULL, 10); /* convert string to long integer */
  }
  gettimeofday(&end, NULL);
  end.tv_sec += 10; /* run 10 seconds */

  if ((pid = fork()) < 0) {
    err_sys("failed to call fork");
  } else if (pid == 0) { /* child proc */
    s = "child";
    printf("current priority coef. in a child %d, set to %d\n",
      nice(0)+nzero, adj);
    errno = 0;
    if ((ret = nice(adj)) == -1 && errno != 0) {
      err_sys("failed to set priority in a child");
    }
    printf("currently nice priority in a child is %d\n", nice(0)+nzero);
  } else {
    s = "parent";
    printf("current nice coef. in a parent is %d\n", nice(0)+nzero);
  }
  for (;;) {
    if (++count == 0)
      err_quit("%s counter overflow", s);
    checktime(s);
  }
}
