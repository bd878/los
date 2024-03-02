#include "lib.h"

static void
sig_catch(int signo)
{
  printf("caught signal\n");
  tty_reset(STDIN_FILENO);
  exit(0);
}

int
main(void)
{
  int i;
  char c;

  if (signal(SIGINT, sig_catch) == SIG_ERR) /* enable signal handling */
    err_sys("failed to call signal(SIGINT)");
  if (signal(SIGQUIT, sig_catch) == SIG_ERR)
    err_sys("failed to call signal(SIGQUIT)");
  if (signal(SIGTERM, sig_catch) == SIG_ERR)
    err_sys("failed to call signal(SIGTERM)");

  if (tty_raw(STDIN_FILENO) < 0)
    err_sys("failed to call tty_raw");
  printf("Shift in raw mode, exit on DELETE press\n");
  while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
    if ((c &= 255) == 0177) /* 0177 = ASCII DELETE */
      break;
    printf("%o\n", c);
  }

  if (tty_reset(STDIN_FILENO) < 0)
    err_sys("failed to call tty_reset");
  if (i <= 0)
    err_sys("failed to read");
  if (tty_cbreak(STDIN_FILENO) < 0)
    err_sys("failed to call tty_cbreak");
  printf("\nShift on cbreak; exit by SIGINT\n");
  while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
    c &= 255;
    printf("%o\n", c);
  }
  if (tty_reset(STDIN_FILENO) < 0)
    err_sys("failed to call tty_reset");
  if (i <= 0)
    err_sys("failed to read");

  exit(0);
}
