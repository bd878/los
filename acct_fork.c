#include "lib.h"

/*
 * Fork and exit child processes to
 * generate process accounting info
 **/
int
main(void)
{
  pid_t pid;

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid != 0) { /* first parent process */
    sleep(2);
    exit(2); /* exit with 2 status code */
  }

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid != 0) { /* first child process */
    sleep(4);
    abort(); /* exit and generate core file */
  }

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid != 0) { /* second child process */
    execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
    exit(7); /* program should not reach this point */
  }

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid != 0) { /* third child process */
    sleep(8);
    exit(0); /* normal exit */
  }

  sleep(6); /* fourth child process */
  kill(getpid(), SIGKILL); /* close by signall without core file */
  exit(6); /* program should not reach this point */
}
