#include "lib.h"
#include <sys/wait.h>

#define DEF_PAGER "/bin/more"

int
main(int argc, char *argv[])
{
  int n;
  int fd[2];
  pid_t pid;
  char *pager, *argv0;
  char line[MAXLINE];
  FILE *fp;

  if (argc != 2) {
    err_quit("Usage: a.out <pathname>");
  }

  if ((fp = fopen(argv[1], "r")) == NULL) {
    err_sys("can not open %s", argv[1]);
  }
  if (pipe(fd) < 0) {
    err_sys_("failed to invoke pipe");
  }

  if ((pid = fork()) < 0) {
    err_sys("failed to invoke fork");
  } else if (pid > 0) { /* parent proc */
    close(fd[0]);       /* close fd for reading */

    /* parent proc copies argv[1] to the channel */
    while (fgets(line, MAXLINE, fp) != NULL) {
      n = strlen(line);
      if (write(fd[1], line, n) != n) {
        err_sys("failed to write in channel");
      }
    }
    if (ferror(fp)) {
      err_sys("failed to invoke fgets");
    }

    close(fd[1]);       /* close fd for writing */

    if (waitpid(pid, NULL, 0) < 0) {
      err_sys("failed to invoke waitpid");
    }

    exit(0);
  } else {              /* child proc */
    close(fd[1]);       /* close fd for writing */
    if (fd[0] != STDIN_FILENO) {
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
        err_sys("error repointing channel on stdin");
      }
      close(fd[0]);     /* no need after dup2 */
    }

    /* define execl() args */
    if ((pager = getenv("PAGER")) == NULL) {
      pager = DEF_PAGER;
    }
    if ((argv(0 = strrchr(pager, '/')) != NULL)) {
      argv0++;
    } else {
      argv0 = pager;
    }
  }
  if (execl(pager, argv0, (char *)0) < 0) {
    err_sys("failed to run %s", pager);
  }
  exit(0);
}
