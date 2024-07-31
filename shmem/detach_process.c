/**
 * detach_process.c - starts new child,
 * that is not bound to calling process's group.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "sync_pipe.h"

// TODO: exit if parent exists too prctl(PR_SET_PDEATHSIG, SIGTERM);

int detach(int fd[2], void (*f)(int *))
{
  pid_t cid;

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid > 0) { /* parent return */
    close(fd[0]); /* read end */

    return cid;
  }
  /* first child */

  pid_t cid2;
  int fd1[2];

  close(fd[1]); /* write end */

  OPEN_PIPE(fd1);

  if ((cid2 = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid2 == 0) { /* second child */
    close(fd1[1]); /* write end */

    WAIT_PARENT(fd1);

    f(fd);

    exit(EXIT_SUCCESS);
  }

  close(fd1[0]); /* read end */

  WAIT_PARENT(fd); /* waits until top parent let continue */
  TELL_CHILD(fd1); /* resume detached */

  exit(EXIT_SUCCESS);
}

/*
 * Necessary to get return status
 **/
int wait_child(int cid) {
  if (waitpid(cid, NULL, 0) < 0) {
    perror("waitpid");
    return -1;
  }
  return 0;
}
