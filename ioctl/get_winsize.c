/*
 * get_winsize.c - obtain window size through
 * ioctl
 */
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

static void pr_winsize(int) {
  struct winsize size;

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) < 0) {
    perror("ioctl");
    exit(EXIT_FAILURE);
  }

  printf("(%d, %d)\n", size.ws_row, size.ws_col);
}

static void pr_sigint(int) {
  printf("bye.\n");
  exit(EXIT_SUCCESS);
}

int main() {
  struct sigaction act, int_act;

  if (isatty(STDIN_FILENO) == 0) {
    perror("isatty");
    exit(EXIT_FAILURE);
  }
  printf("stdin is terminal. ok.\n");

  act.sa_handler = pr_winsize;
  if (sigemptyset(&act.sa_mask) < 0) {
    perror("sigemptyset");
    exit(EXIT_FAILURE);
  }
  act.sa_flags |= SA_RESTART;

  if (sigaction(SIGWINCH, &act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  int_act.sa_handler = pr_sigint;
  if (sigemptyset(&int_act.sa_mask) < 0) {
    perror("sigemptyset");
    exit(EXIT_FAILURE);
  }

  if (sigaction(SIGINT, &int_act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  printf("waiting for WINCH events...\n");
  for (;;)
    pause();

  exit(EXIT_SUCCESS);
}
