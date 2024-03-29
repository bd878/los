#define _GNU_SOURCE
#include <sys/capability.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* gcc -lcap */

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
                     } while (0)

static int /* Startup function for cloned child */
childFunc(void *arg)
{
  cap_t caps;

  for (;;) {
    printf("eUID = %ld; eGID = %ld; ",
      (long) geteuid(), (long) getegid());

    caps = cap_get_proc();
    printf("capabilities: %s\n", cap_to_text(caps, NULL));

    if (arg == NULL)
      break;

    sleep(5);
  }

  return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE]; /* Space for child's stack */

int
main(int argc, char *argv[])
{
  pid_t pid;

  /* Create child; child commences execution in childFunc() */

  pid = clone(childFunc, child_stack + STACK_SIZE,
    CLONE_NEWUSER | SIGCHLD, argv[1]);
  if (pid == -1)
    errExit("clone");

  /* Parent falls through to here. Child wait */

  if (waitpid(pid, NULL, 0) == -1)
    errExit("waitpid");

  exit(EXIT_SUCCESS);
}