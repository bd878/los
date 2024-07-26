/**
 * detach_process.c - starts new child,
 * that is not bound to calling process's group.
 * Sends its pid_t via provided pipe
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "sync_pipe.h"

void detach(int fd[2], void (*f)(int *))
{
  pid_t cid;

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid > 0) { /* parent return */
    close(fd[1]); /* write end */

    printf("[%d] waiting child to finish: %d\n", getpid(), cid);
    if (waitpid(cid, NULL, 0) < 0) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }
    printf("[%d] finished %d\n", getpid(), cid);

    return;
  }
  /* first child */

  pid_t cid2;
  int fd1[2];

  close(fd[0]); /* read end */

  OPEN_PIPE(fd1);

  if ((cid2 = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid2 == 0) { /* second child */
    pid_t me;

    close(fd1[0]); /* read end */

    TELL_PARENT(fd1);

    me = getpid();

    if (write(fd[1], &me, sizeof(pid_t)) < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    f(fd);

    exit(EXIT_SUCCESS);
  }

  close(fd1[1]); /* write end */

  WAIT_CHILD(fd1);

  exit(EXIT_SUCCESS); 
}

int wait_child(int fd[2], pid_t* cid)
{
  if (read(fd[0], cid, sizeof(*cid)) < 0) {
    fprintf(stderr, "[%d] read error: %s\n", getpid(), strerror(errno));
    exit(EXIT_FAILURE);
  }
}

