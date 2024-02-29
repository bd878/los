#ifndef __TTY_ICANON_H
#define __TTY_ICANON_H

#include <termios.h>

int tty_cbreak(int); /* turn tty in cbreak (per-char) mode */
int tty_raw(int); /* turn terminal in raw (opaque) mode */
int tty_reset(int); /* restore tty state */
void tty_atexit(void); /* atexit(tty_atexit) */
struct termios *tty_termios(void); /* learn initial tty state */

#endif
