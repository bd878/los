#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

/*
 * Custom implementation
 * of system func
 * without signals handling
 **/
int
system_no_signal(const char *cmdstring)
{
  pid_t pid;
  int status;

  if (cmdstring == NULL) {
    return(1); /* UNIX always has a command interpreter */
  }

  if ((pid = fork()) < 0) {
    status = -1; /* proc limit exceeded */
  } else if (pid == 0) { /* child proc */
    execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
    _exit(127); /* error calling execl */
  } else {
    while (waitpid(pid, &status, 0) < 0) {
      if (errno != EINTR) {
        status = -1; /* waitpid returned error except EINTR */
        break;
      }
    }
  }

  return(status);
}
