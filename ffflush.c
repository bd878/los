#include "lib.h"
#include <sys/wait.h>

int
main(void)
{
  char line[MAXLINE];
  FILE *fpin;

  if ((fpin = popen("./myuclc", "r")) == NULL)
    err_sys("failed to call popen");

  for ( ; ; ) {
    fputs("prompt> ", stdout);
    fflush(stdout);
    if (fgets(line, MAXLINE, fpin) == NULL) /* reading from a channel */
      break;
    if (fputs(line, stdout) == EOF)
      err_sys("failed to call fputs");
  }
  if (pclose(fpin) == -1)
    err_sys("failed to call pclose");

  putchar('\n');
  exit(0);
}