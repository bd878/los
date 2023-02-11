#ifndef __pty_fork_h
#define __pty_fork_h

pid_t pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
  const struct termios *slave_termios,
  const struct winsize *slave_winsize);

#endif