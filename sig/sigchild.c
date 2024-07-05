/*
 * sigchild.c - send SIGHUP to child process.
 * Catch SIGCHLD in parent process.
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void child_handler(int);
void parent_handler(int);

int main() {
  pid_t cid, pid;
  int status;
  struct sigaction act_hup, act_cld;

  act_hup.sa_handler = &child_handler;
  if (sigaction(SIGHUP, &act_hup, NULL) < 0) {
    perror("sigaction");
    exit(1);
  }

  act_cld.sa_handler = &parent_handler;
  if (sigaction(SIGCHLD, &act_cld, NULL) < 0) {
    perror("sigaction");
    exit(1);
  }

  pid = getpid();
  printf("parent id = %d\n", pid);

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(1);
  } else if (cid == 0) { /* child */
    printf("child waits signal\n");
    pause();
    printf("child resumed\n");
    exit(0);
  }

  printf("child id = %d\n", cid);

  printf("parent executing...\n");
  for (int i = 0; i < 5; i++) {
    printf("%d: %d\n", pid, i);
    sleep(1);
  }
  printf("SIGHUP sent\n");
  if (kill(cid, SIGHUP) < 0) {
    perror("kill");
    exit(1);
  }

  printf("parent executing 2...\n");
  if (wait(&status) < 0) {
    if (WIFEXITED(status)) {
      printf("exit status: %d\n", WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status)) {
      printf("exit signal: %d\n", WTERMSIG(status));
    }
    if (WIFSTOPPED(status)) {
      printf("stop signal: %d\n", WSTOPSIG(status));
    }
    if (WIFCONTINUED(status)) {
      printf("continued\n");
    }
  }

  printf("parent exits\n");
  return 0;
}

void child_handler(int signo)
{
  pid_t pid, ppid;

  ppid = getppid();
  pid = getpid();

  printf("received signal %d\n", signo);
  printf("ppid: %d; pid: %d\n", ppid, pid);
}

void parent_handler(int signo)
{
  pid_t pid;

  pid = getpid();

  printf("parent handler, pid = %d\n", pid);
}
