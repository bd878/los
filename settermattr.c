#include "lib.h"
#include <termios.h>

int
main(void)
{
  struct termios term;

  if (tcgetattr(STDIN_FILENO, &term) < 0)
    err_sys("fail to call tcgetattr");

  switch (term.c_cflag & CSIZE) {
  case CS5:
    printf("5 bits per byte\n");
    break;
  case CS6:
    printf("6 bits per byte\n");
    break;
  case CS7:
    printf("7 bits per byte\n");
    break;
  case CS8:
    printf("8 bits per byte\n");
    break;
  default:
    printf("unknown bit quantity per byte");
  }

  term.c_cflag &= CSIZE; /* zeroe bits */
  term.c_cflag |= CS8; /* set 8 bits per byte */
  if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
    err_sys("failed to call tcsetattr");

  exit(0);
}
