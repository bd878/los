#include "lib.h"

#define BUFFSIZE 512

static void sig_term(int);
static volatile sig_atomic_t sigcaught; /* signal handler changing */

void
loop(int ptym, int ignoreeof)
{
  pid_t child;
  int nread;
  char buf[BUFFSIZE];

  if ((child = fork()) < 0) {
    err_sys("failed to call fork");
  } else if (child == 0) { /* child proc copies stdin to ptym */
    for ( ; ; ) {
      if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
        err_sys("failed to read stdin");
      else if (nread == 0)
        break; /* EOF in stdin designates input end */
      if (writen(ptym, buf, nread) != nread)
        err_sys("failed to write to master pty");
    }

    if (ignoreeof == 0)
      kill(getppid(), SIGTERM);
    exit(0);
  }

  if (signal_intr(SIGTERM, sig_term) == SIG_ERR)
    err_sys("failed to call signal_intr for SIGTERM");
  for ( ; ; ) {
    if ((nread = read(ptym, buf, BUFFSIZE)) <= 0)
      break;
    if (writen(STDOUT_FILENO, buf, nread) != nread)
      err_sys("failed to write to stdout");
  }

  if (sigcaught == 0)
    kill(child, SIGTERM);
}

static void
sig_term(int signo)
{
  sigcaught = 1;
}