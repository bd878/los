#include "pty_open.h"
#include "lib.h"
#include <termios.h>

pid_t
pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
  const struct termios *slave_termios,
  const struct winsize *slave_winsize)
{
  int fdm, fds;
  pid_t pid;
  char pts_name[20];

  if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0)
    err_sys("failed to open master pty: %s, error: %d", pts_name, fdm);

  if (slave_name != NULL) {
    /*
     * Return leading device name.
     * Terminate by \0 symbol,
     * when strlen(ptr) > pts_namesz.
     */
    strncpy(slave_name, pts_name, slave_namesz);
    slave_name[slave_namesz - 1] = '\0';
  }

  if ((pid = fork()) < 0) {
    return(-1);
  } else if (pid == 0) { /* child proc */
    if (setsid() < 0)
      err_sys("failed to call setsid");

    /*
     * System V automaticall set master terminal
     * on open
     */
    if ((fds = ptys_open(pts_name)) < 0)
      err_sys("failed to open slave pty");
    close(fdm); /* work with master pty on child proc is terminated */

#if defined(BSD)
    /**
     * TIOCSCITY - way to assign master terminal in BSD
     */
    if (opctl(fds, TIOCSCITY, (char *)0) < 0)
      err_sys("failed to call TIOCSCITY");
#endif
    /**
     * Init slave pty termios and winsize structs
     */
    if (slave_termios != NULL) {
      if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
        err_sys("failed to call tcsetattr for slave pty");
    }
    if (slave_winsize != NULL) {
      if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
        err_sys("failed to exec TIOCSWINSZ for slave pty");
    }

    /**
     * Connect stdin/out/err with pty in child proc
     */
    if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
      err_sys("error calling dup2 for stdin");
    if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
      err_sys("error calling dup2 for stdout");
    if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
      err_sys("error calling dup2 for stderr");
    if (fds != STDIN_FILENO && fds != STDOUT_FILENO &&
      fds != STDERR_FILENO)
      close(fds);
    return(0); /* return 0 to child proc like fork() */
  } else {
    /* parent proc */
    *ptrfdm = fdm; /* return master pty fd */
    return(pid); /* return slave pid to master */
  }
}