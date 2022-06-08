#include "lib.h"

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
  if (writen(fd, line, n) != n) {
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
