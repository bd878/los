/**
 * write_shmem.c - starts two non-sibling processes,
 * attaches shared memory segment in each
 * and updates a value between them.
 */
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

void sig_handler(int);

int main() {
  pid_t me, cid, cid2;
  int fd[2];

  me = getpid();
  printf("me: %d\n", me);

  if (pipe(fd) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid == 0) { /* first child */
    pid_t ppid1, me1, cid2;
    char c;
    int fd1[2];

    close(fd[0]); /* read end */

    if (pipe(fd1) < 0) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    me1 = getpid();
    ppid1 = getppid();

    printf("[%d] ppid: %d\n", me1, ppid1);

    if ((cid2 = fork()) < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (cid2 == 0) { /* second child */
      pid_t ppid2, me2;
      struct sigaction sig;

      close(fd1[0]); /* read end */

      me2 = getpid();
      ppid2 = getppid();

      printf("[%d] ppid: %d\n", me2, ppid2);
      printf("[%d] waiting parent...\n", me2);

      if (write(fd1[1], "c", 1) < 0) { /* waits for parent */
        perror("write");
        exit(EXIT_FAILURE);
      }

      sig.sa_handler = sig_handler;
      sigemptyset(&sig.sa_mask);

      if (sigaction(SIGHUP, &sig, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
      }

      printf("[%d] waiting for SIGHUP...\n", me2);
      pause();
      // ppid is not initial parent id cid1
      printf("[%d] ppid: %d\n", me, getppid());
      printf("[%d] exit due to signal\n", me2);

      exit(EXIT_SUCCESS);
    }

    close(fd1[1]); /* write end */

    if (read(fd1[0], &c, 1) < 0) {
      fprintf(stderr, "[%d] read error: %s\n", me1, strerror(errno));
      exit(EXIT_FAILURE);
    }

    printf("[%d] started child %d\n", me1, cid2);

    if (write(fd[1], &cid2, sizeof(pid_t)) < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    printf("[%d] exit.\n", me1);

    exit(EXIT_SUCCESS);
  }

  close(fd[1]); /* write end */

  if (read(fd[0], &cid2, sizeof(cid2)) < 0) {
    fprintf(stderr, "[%d] read error: %s\n", me, strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("[%d] started child %d\n", me, cid);
  printf("[%d] waiting child %d...\n", me, cid);

  if (waitpid(cid, NULL, 0) < 0) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }

  printf("[%d] send SIGHUP to child %d\n", me, cid2);
  if (kill(cid2, SIGHUP) < 0) {
    perror("kill");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

void sig_handler(int sig)
{
  printf("[%d] got signal %s\n", getpid(), strsignal(sig));
}
