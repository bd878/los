#include "pr_exit.h"
#include "lib.h"

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

  if (wait(&status) != pid) {
    printf("failed to call wait\n");
    exit(1);
  }
  pr_exit(status);

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid == 0) {
    abort();
  }

  if (wait(&status) != pid) {
    printf("failed to call wait\n");
    exit(1);
  }
  pr_exit(status);

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
