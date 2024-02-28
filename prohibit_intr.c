#include "lib.h"
#include <termios.h>

int
main(void)
{
  struct termios term;
  long vdisable;

  if (isatty(STDIN_FILENO) == 0)
    err_quit("standard input device is not a terminal");

  if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
    err_quit("fpathconf error or _POSIX_VDISABLE is not supported");

  /* get terminal options */
  if (tcgetattr(STDIN_FILENO, &term) < 0)
    err_sys("failed to call tcgetattr");

  term.c_cc[VINTR] = vdisable; /* prohibit INTR special char */
  term.c_cc[VEOF] = 2;         /* EOF is Ctrl-B now */

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
    err_sys("error calling tcsetattr");

  exit(0);
}
