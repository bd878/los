#include <sys/wait.h>
#include "lib.h"

void
pr_exit(int status)
{
  if (WIFEXITED(status))
    printf("normal termination, exit code = %d\n",
      WEXITSTATUS(status));
  else if (WIFSIGNALED(status))
    printf("emergency exit, signal code = %d%s\n",
      WTERMSIG(status),
#ifdef WCOREDUMP
      WCOREDUMP(status) ? " (core file created)" : "");
#else
      "");
#endif
  else if (WIFSTOPPED(status))
    printf("child process stopped, signal code = %d\n",
      WSTOPSIG(status));
}
