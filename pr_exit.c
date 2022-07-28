#include <sys/wait.h>
#include "lib.h"

/*
 * wait and waitpid functions
 * return exited child pid on success
 * or -1 on error.
 * They recieve pointer on status variable
 * that is written in child process
 * exit status.
 * To interpret statloc, POSIX
 * provides those macroextentions
 **/
void
pr_exit(int status)
{
  if (WIFEXITED(status)) /* child process exited normally */
    printf("normal termination, exit code = %d\n",
      WEXITSTATUS(status));
  else if (WIFSIGNALED(status)) /* status code is recieved on emergency exit */
    printf("emergency exit, signal code = %d%s\n",
      WTERMSIG(status),
#ifdef WCOREDUMP
      WCOREDUMP(status) ? " (core file created)" : "");
#else
      "");
#endif
  else if (WIFSTOPPED(status)) {
    /* statis code is recieved if */
    /* child process terminated by signal */
    printf("child process stopped, signal code = %d\n",
      WSTOPSIG(status)); /* returns signal id that made child process terminate */
  }
}
