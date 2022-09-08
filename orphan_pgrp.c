#include "lib.h"
#include <errno.h>

static void sig_hup();

static void pr_ids(char *);

int
main(void)
{
  char c;
  pid_t pid;

  pr_ids("parent");
  if ((pid = fork()) < 0) {
    err_sys("failed to call fork");
  } else if (pid > 0) { /* parent process */
    sleep(5); /* pause to let child execute first */
  } else { /* child process */
    pr_ids("child");
    signal(SIGHUP, sig_hup); /* set signal handler */
    kill(getpid(), SIGTSTP); /* pause itseld */
    pr_ids("child"); /* print data, */
                     /* when process resumed */
    if (read(STDIN_FILENO, &c, 1) != 1)
      printf("failed to read controlling terminal, errno = %d\n",
        errno);
  }

  exit(0);
}

static void
sig_hup()
{
  printf("SIGHUP caught, pid = %ld\n", (long)getpid());
}

static void
pr_ids(char *name)
{
  printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
    name, (long)getpid(), (long)getppid(), (long)getpgrp(),
    (long)tcgetpgrp(STDIN_FILENO));
  fflush(stdout);
}
