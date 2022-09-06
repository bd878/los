#include "lib.h"
#include <sys/times.h>

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

/*
 * Per argument command
 * measures startup time
 * and finish time,
 * tells overall time (real),
 * sys time, and user time
 **/
int
main(int argc, char *argv[])
{
  int i;
  setbuf(stdout, NULL);
  for (i = 1; i < argc; i++) {
    do_cmd(argv[i]); /* one time per each command line argument */
  }

  exit(0);
}

static void
do_cmd(char *cmd) /* starts and measures "cmd" run time */
{
  struct tms tmsstart, tmsend;
  clock_t start, end;
  int status;

  printf("\ncommand: %s\n", cmd);

  if ((start = times(&tmsstart)) == -1) /* initial values */
    err_sys("failed to call times");

  if ((status = system(cmd)) < 0) /* run cmd */
    err_sys("failed to run system()");

  if ((end = times(&tmsend)) == -1) /* final values */
    err_sys("failed to call times");

  pr_times(end - start, &tmsstart, &tmsend);
  pr_exit(status);
}

static void
pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
  static long clktck = 0;

  if (clktck == 0) /* get ticks per second the first time */
    if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
      err_sys("failed to call sysconf");

  printf(" real: %7.2f\n", real / (double) clktck); /* real is the time from start to stop */
  printf(" user: %7.2f\n", /* program commands execution time */
    (tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
  printf(" sys: %7.2f\n", /* core commands execution time */
    (tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
  printf(" child user: %7.2f\n",
    (tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
  printf(" child sys: %7.2f\n",
    (tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);
}
