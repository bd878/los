#include "lib.h"

static int pfd1[2], pfd2[2];

void
TELL_WAIT(void)
{
  if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
    err_sys("failed to call pipe");
}

void
TELL_PARENT(pid_t pid)
{
  if (write(pfd2[1], "c", 1) != 1)
    err_sys("failed to call write");
}

void
WAIT_PARENT(void)
{
  char c;

  if (read(pfd1[0], &c, 1) != 1)
    err_sys("failed to call read");

  if (c != 'p')
    err_quit("WAIT_PARENT: wrong data");
}

void
TELL_CHILD(pid_t pid)
{
  if (write(pfd1[1], "p", 1) != 1)
    err_sys("failed to call write");
}

void
WAIT_CHILD(void)
{
  char c;

  if (read(pfd2[0], &c, 1) != 1)
    err_sys("failed to call read");

  if (c != 'c')
    err_quit("WAIT_CHILD: wrong data");
}
