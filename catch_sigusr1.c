#include "lib.h"

static void sig_usr(int); /* one handler for two signals */

/*
 * ./a.out *
 * kill -USR1 pid
 * kill -USR2 pid
 * kill pid
 **/
int
main(void)
{
  if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
    printf("cannot catch signal SIGUSR1\n");
    exit(1);
  }
  if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
    printf("cannot catch signal SIGUSR2\n");
    exit(1);
  }
  for ( ; ; )
    pause();
}

static void
sig_usr(int signo) /* arg is signal num */
{
  if (signo == SIGUSR1) {
    printf("SIGUSR1 accepted\n");
  } else if (signo == SIGUSR2) {
    printf("SIGUSR2 accepted\n");
  } else {
    err_dump("signal %d\n", signo);
  }
}
