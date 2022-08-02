#include "lib.h"
#include <sys/wait.h>

/*
 * (Long running) parent process may 
 * fork a child processes, but
 * do not wait until it exits
 * to recieve exit status.
 * Child process would
 * become a zombie. 
 * To prevent zombies, fork twice
 * to make child process adopt
 * from init proc. Init proc always
 * recieves child proc exit statuses
 * (hence does not make
 * them a Zombie)
 **/
int
main()
{
  pid_t pid;

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid == 0) { /* first child */
    if ((pid = fork()) < 0) {
      printf("failed to call fork\n");
      exit(1);
    } else if (pid > 0) {
      printf("second child process id = %ld\n", (long)pid);
      exit(0); /* exit first child. */
               /* it is the parent process for the second child */
    }

    sleep(2);
    printf("second child continues, parent proc id = %ld\n",
      (long)getppid()); /* must be 1 (init) or systemd */
    exit(0);
  }

  if (waitpid(pid, NULL, 0) != pid) { /* wait until first child exits */
    printf("failed to call waitpid\n");
    exit(1);
  }

  printf("first child id = %ld\n", (long)pid);

  /* Parent process continues here */
  /* since it is not a parent for */
  /* the second process anymore */
  exit(0);
}
