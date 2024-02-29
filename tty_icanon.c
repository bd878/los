#include <termios.h>
#include <errno.h>
#include "tty_icanon.h"
#include "lib.h"

static struct termios save_termios;
static int            ttysavefd = -1;
static enum { RESET, RAW, CBREAK } ttystate = RESET;

int
tty_cbreak(int fd)
{
  int             err;
  struct termios  buf;

  if (ttystate != RESET) {
    errno = EINVAL;
    return(-1);
  }
  if (tcgetattr(fd, &buf) < 0)
    return(-1);
  save_termios = buf; /* struct copy */

  /*
   * Turn off echo-output and exit canon mode
   */
  buf.c_lflag &= (ECHO | ICANON);

  /*
   * B case: 1 byte at least, timout unlimited
   */
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;
  if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    return(-1);

  /*
   * All changes are applied?
   * tcsetattr may return 0 sogar on partial update
   */
  if (tcgetattr(fd, &buf) < 0) {
    err = errno;
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = err;
    return(-1);
  }
  if ((buf.c_lflag & (ECHO | ICANON)) || buf.c_cc[VMIN] != 1 ||
    buf.c_cc[VTIME] != 0) {
    /*
     * Updates are not applied entierely so far.
     * Reset initial settings.
     */
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = EINVAL;
    return(-1);
  }

  ttystate = CBREAK;
  ttysavefd = fd;
  return(0);
}

int
tty_raw(int fd)
{
  int            err;
  struct termios buf;

  if (ttystate != RESET) {
    errno = EINVAL;
    return(-1);
  }

  if (tcgetattr(fd, &buf) < 0)
    return(-1);
  save_termios = buf; /* struct copy */

  /*
   * Turn off echo-output, exit canon mode,
   * turn off extended input handlung, ...
   */
  buf.c_lflag &= (ECHO | ICANON | IEXTEN | ISIG);

  /*
   * Do not emit SIGINT signal by BREAK command,
   * turn off CR->NL switch, ...
   */
  buf.c_iflag &= (BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  /*
   * Reset size-controlling mask, setoff parity bit
   */
  buf.c_cflag &= (CSIZE | PARENB);

  /*
   * Char symbol is 8 bits/char now
   */
  buf.c_cflag |= CS8;

  /*
   * Turn off output handlung
   */
  buf.c_oflag &= OPOST;

  /*
   * B case: 1 byte at least, timout unlimited
   */
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;
  if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    return(-1);

  /*
   * All changes are applied?
   * tcsetattr may return 0 sogar on partial update
   */
  if (tcgetattr(fd, &buf) < 0) {
    err = errno;
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = err;
    return(-1);
  }

  if (
    (buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
    (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
    (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
    (buf.c_oflag & OPOST) || buf.c_cc[VMIN] != 1 ||
    buf.c_cc[VTIME] != 0
  ) {
    /*
     * Partiall updates only. Reset initial settings
     */
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = EINVAL;
    return(-1); 
  }

  ttystate = RAW;
  ttysavefd = fd;
  return(0);
} 

int
tty_reset(int fd)
{
  if (ttystate == RESET)
    return(0);
  if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
    return(-1);
  ttystate = RESET;

  return(0);
}

void
tty_atexit(void)
{
  if (ttysavefd >= 0)
    tty_reset(ttysavefd);
}

struct termios*
tty_termios(void)
{
  return(&save_termios);
}
