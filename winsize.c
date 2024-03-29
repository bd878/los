#include "lib.h"
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

static void
pr_winsize(int fd)
{
  struct winsize size;

  if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
    err_sys("failed to call TIOCGWINSZ");
  printf("%d rows, %d cols in row\n", size.ws_row, size.ws_col);
}

static void
sig_winch(int signo)
{
  printf("got signal SIGWINCH\n");
  pr_winsize(STDIN_FILENO);
}

int
main(void)
{
  if (isatty(STDIN_FILENO) == 0)
    exit(1);
  if (signal(SIGWINCH, sig_winch) == SIG_ERR)
    err_sys("failed to call signal");
  pr_winsize(STDIN_FILENO); /* output initial win sizes */
  for (;;) /* halt */
    pause();
}
