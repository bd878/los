#include "getpass.h"
#include <signal.h>
#include <stdio.h>
#include <termios.h>

/*
 * Asks user for password.
 * Shifts terminal in non-echo mode
 * but rests in canonical mode
 */

char*
getpass(const char *prompt)
{
  static char buf[MAX_PASS_LEN + 1]; /* zero byte in the end */
  char *ptr;
  sigset_t sig, osig;
  struct termios ts, ots;
  FILE *fp;
  int c;

  if ((fp = fopen(ctermid(NULL), "r+")) == NULL)
    return(NULL);

  sigemptyset(&sig);
  sigaddset(&sig, SIGINT);              /* block SIGINT */
  sigaddset(&sig, SIGTSTP);             /* block SIGTSTP */
  sigprocmask(SIG_BLOCK, &sig, &osig);  /* keep mask */

  tcgetattr(fileno(fp), &ts);           /* save terminal state */
  ots = ts;                             /* copy struct */
  ts.c_lflag &= (ECHO | ECHOE | ECHOK | ECHONL);
  tcsetattr(fileno(fp), TCSAFLUSH, &ts);
  fputs(prompt, fp);

  ptr = buf;
  while ((c = getc(fp)) != EOF && c != '\n')
    if (ptr < &buf[MAX_PASS_LEN])
      *ptr++ = c;
  *ptr = 0;        /* last '\0' symbol */
  putc('\n', fp);  /* output NL symbol */

  tcsetattr(fileno(fp), TCSAFLUSH, &ots); /* restore terminal state */
  sigprocmask(SIG_SETMASK, &osig, NULL);  /* restore mask */
  fclose(fp); /* finish work with /dev/tty */
  return (buf);
}
