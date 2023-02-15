#include "lib.h"
#include <limits.h>
#include <sys/resource.h>

static void err_doit(int, int, const char *, va_list);

static int ttysavefd = -1;

static struct termios   save_termios;

static enum { RESET, RAW, CBREAK } ttystate = RESET;

/* random value provided OPEN_MAX is not defined */
#define OPEN_MAX_GUESS 256

int
Socket(int family, int type, int protocol) {
  int ret;

  if ((ret = socket(family, type, protocol)) < 0) {
    perror("socket");
    exit(1);
  }
  return ret;
}

void
Bind(int fd, const struct sockaddr *addr, size_t len) {
  if (bind(fd, addr, len) < 0) {
    perror("bind");
    exit(1);
  }
}

void
Listen(int fd, int backlog) {
  if (listen(fd, backlog) < 0) {
    perror("listen");
    exit(1);
  }
}

int
Accept(int fd, struct sockaddr *addr, socklen_t *len) {
  int ret;

  if ((ret = accept(fd, addr, len)) < 0) {
    perror("accept");
    exit(1);
  }
  return ret;
}

void
Connect(int fd, const struct sockaddr *addr, socklen_t len) {
  if (connect(fd, addr, len) == -1) {
    perror("connect");
    exit(1);
  }
}

pid_t
Fork() {
  pid_t p;

  if ((p = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  return p;
}

void
Close(int fd) {
  if (close(fd) == -1) {
    perror("close");
    exit(1);
  }
}

void
Inet_pton(int family, const char *str, void *addr) {
  if (inet_pton(family, str, addr) == -1) {
    perror("inet_pton");
    exit(1);
  }
}

ssize_t
writen(int fd, const void *line, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *vptr;

  vptr = (const char *)line;
  nleft = n;
  while (nleft > 0) {
    if ((nwritten = write(fd, vptr, n)) <= 0) {
      if (nwritten == -1 && errno == EINTR) { /* a signal interrupted write call */
        nwritten = 0; /* try again */
      } else {
        return -1;
      }
    }
    nleft -= nwritten;
    vptr += nwritten;
  }
  return n;
}

void
Writen(int fd, const void *line, size_t n) {
  if (writen(fd, line, n) != (int)n) {
    perror("writen");
    exit(1);
  }
}

char *
Fgets(char *str, int n, FILE *stream) {
  char *vptr;

  if ((vptr = fgets(str, n, stream)) == NULL && ferror(stream)) {
    perror("fgets");
    exit(1);
  }
  return vptr;
}

void
Fputs(const char *str, FILE *stream) {
  if (fputs(str, stream) == EOF) {
    perror("fputs");
    exit(1);
  }
}

FILE *
Fopen(const char *pathname, const char *mode) {
  FILE *fs;

  if ((fs = fopen(pathname, mode)) == NULL) {
    perror("fopen");
    exit(1);
  }

  return fs;
}

FILE *
Freopen(const char *pathname, const char *mode, FILE *stream) {
  FILE *fs;

  if ((fs = freopen(pathname, mode, stream)) == NULL) {
    perror("freopen");
    exit(1);
  }

  return fs;
}

FILE *
Fdopen(int fd, const char *mode) {
  FILE *fs;

  if ((fs = fdopen(fd, mode)) == NULL) {
    perror("fdopen");
    exit(1);
  }

  return fs;
}

Sigfunc *
Signal(int signo, Sigfunc *func) {
  struct sigaction act, oldact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGALRM) { /* interrupted io operations must die */
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
  } else {
#ifdef SA_RESTART
    act.sa_flags |= SA_RESTART; /* interrupted calls must restart */
#endif
  }
  if (sigaction(signo, &act, &oldact) < 0) {
    return SIG_ERR;
  }
  return oldact.sa_handler;
}

void
Setfl(int fd, int flags) {
  int val;

  if ((val = fcntl(fd, F_GETFL)) < 0) {
    perror("fcntl");
    exit(1);
  }

  val |= flags;

  if (fcntl(fd, F_SETFL, val) < 0) {
    perror("fcntl");
    exit(1);
  }
}

void
Clrfl(int fd, int flags) {
  int val;

  if ((val = fcntl(fd, F_GETFL)) < 0) {
    perror("fcntl");
    exit(1);
  }

  val &= ~flags;

  if (fcntl(fd, F_SETFL, val) < 0) {
    perror("fcntl");
    exit(1);
  }
}

ssize_t
readn(int fd, void *vptr, size_t nbytes) {
  char *ptr;
  size_t nleft;
  ssize_t n;

  ptr = vptr;
  nleft = nbytes;
  while (nleft > 0) {
    n = read(fd, ptr, nleft);
    if (n == -1) {
      if (errno == EINTR) {
        n = 0;
      } else {
        return -1;
      }
    } else if (n == 0) {
      break; /* EOF reached */
    }

    ptr += n;
    nleft -= n;
  }
  return (n - nleft);
}

ssize_t
Readn(int fd, void *buf, size_t nbytes) {
  ssize_t n;

  if ((n = readn(fd, buf, nbytes)) < 0) {
    perror("readn");
    exit(1);
  }
  return n;
}

int
Select(int maxfdp, fd_set *rset, fd_set *wset, fd_set *errset, struct timeval *timeout) {
  int n;
  if ((n = select(maxfdp, rset, wset, errset, timeout)) < 0) {
    perror("select");
    exit(1);
  }
  return n;
}

int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout) {
  int n;

  if ((n = poll(fdarray, nfds, timeout)) < 0) {
    perror("poll");
    exit(1);
  }
  return n;
}

ssize_t
Read(int fd, void *buf, size_t count) {
  ssize_t n;

  if ((n = read(fd, buf, count)) == -1) {
    perror("read");
  }
  return n;
}

void
Shutdown(int fd, int how) {
  if (shutdown(fd, how) == -1) {
    perror("shutdown");
    exit(1);
  }
}

ssize_t
Write(int fd, const void *buf, size_t ncount) {
  ssize_t n;

  if ((n = write(fd, buf, ncount)) == -1) {
    perror("write");
    exit(1);
  }
  return n;
}

long
Sysconf(int name) {
  long result = 0;
  errno = 0;
  if ((result = sysconf(name)) == -1 && errno != 0) {
    perror("Sysconf");
  }
  return result;
}

void
Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr) {
  if (getsockopt(fd, level, optname, optval, optlenptr) < 0) {
    perror("getsockopt");
  }
}

long
open_max(void)
{
  long openmax;
  struct rlimit rl;

  if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 ||
    openmax == LONG_MAX) {
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
      err_sys("can't get file limit");
    if (rl.rlim_max == RLIM_INFINITY)
      openmax = OPEN_MAX_GUESS;
    else
      openmax = rl.rlim_max;
  }
  return(openmax);
}

void
err_dump(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
  abort(); /* dump core and terminate */
  exit(1); /* should never reach */
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate
 **/
void
err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
  exit(1);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 **/
void
err_quit(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, 0, fmt, ap);
  va_end(ap);
  exit(1);
}

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explicit parameter.
 * Print a message and terminate.
 **/
void
err_exit(int error, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, error, fmt, ap);
  va_end(ap);
  exit(1);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag"
 **/
static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
  char buf[MAXLINE];

  vsnprintf(buf, MAXLINE-1, fmt, ap);
  if (errnoflag) {
    snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
      strerror(error));
  }
  strcat(buf, "\n");
  fflush(stdout); /* if stdout and stderr are the same */
  fputs(buf, stderr);
  fflush(NULL); /* fflush all stdio output streams */
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void
err_msg(const char *fmt, ...)
{
  va_list   ap;

  va_start(ap, fmt);
  err_doit(0, 0, fmt, ap);
  va_end(ap);
}

int
fd_pipe(int fd[2])
{
  return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}

Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
  struct sigaction  act, oact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
#ifdef  SA_INTERRUPT
  act.sa_flags |= SA_INTERRUPT;
#endif
  if (sigaction(signo, &act, &oact) < 0)
    return(SIG_ERR);
  return(oact.sa_handler);
}

int
tty_raw(int fd)
{
  int       err;
  struct termios  buf;

  if (ttystate != RESET) {
    errno = EINVAL;
    return(-1);
  }
  if (tcgetattr(fd, &buf) < 0)
    return(-1);
  save_termios = buf; /* structure copy */

  /*
   * Echo off, canonical mode off, extended input
   * processing off, signal chars off.
   */
  buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  /*
   * No SIGINT on BREAK, CR-to-NL off, input parity
   * check off, don't strip 8th bit on input, output
   * flow control off.
   */
  buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  /*
   * Clear size bits, parity checking off.
   */
  buf.c_cflag &= ~(CSIZE | PARENB);

  /*
   * Set 8 bits/char.
   */
  buf.c_cflag |= CS8;

  /*
   * Output processing off.
   */
  buf.c_oflag &= ~(OPOST);

  /*
   * Case B: 1 byte at a time, no timer.
   */
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;
  if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    return(-1);

  /*
   * Verify that the changes stuck.  tcsetattr can return 0 on
   * partial success.
   */
  if (tcgetattr(fd, &buf) < 0) {
    err = errno;
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = err;
    return(-1);
  }
  if ((buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
    (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
    (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
    (buf.c_oflag & OPOST) || buf.c_cc[VMIN] != 1 ||
    buf.c_cc[VTIME] != 0) {
    /*
     * Only some of the changes were made.  Restore the
     * original settings.
     */
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = EINVAL;
    return(-1);
  }

  ttystate = RAW;
  ttysavefd = fd;
  return(0);
}

struct termios *
tty_termios(void)
{
  return(&save_termios);
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