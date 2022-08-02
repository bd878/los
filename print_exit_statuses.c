#include "pr_exit.h"
#include "lib.h"

/*
 * Prints exit statuses
 * of child processes
 * that exit in a different manner.
 * wait syscall waits the first
 * child process to exit and writes
 * exit status to the
 * provided int pointer
 * wait syscall also blocks the caller
 * until child process exits
 **/
int
main(void)
{
  pid_t pid;
  int status;

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid == 0) /* child process */
    exit(7);

  if (wait(&status) != pid) { /* wait child process to finish */
    printf("failed to call wait\n");
    exit(1);
  }
  pr_exit(status); /* print child process exit status */

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid == 0) { /* child process */
    abort(); /* sent SIGABRT */
  }

  if (wait(&status) != pid) { /* wait child process to finish */
    printf("failed to call wait\n");
    exit(1);
  }
  pr_exit(status); /* print exit status */

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid == 0) {
    status /= 0; /* generate SIGFPE */
  }

  if (wait(&status) != pid) {
    printf("failed to call wait\n");
    exit(1);
  }
  pr_exit(status);

  exit(0);
}
