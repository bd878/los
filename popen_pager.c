#include "lib.h"
#include <sys/wait.h>

#define PAGER "${PAGER:-more}" /* PAGER env var or default (more) */

int
main(int argc, char *argv[])
{
  char line[MAXLINE];
  FILE *fpin, *fpout;

  if (argc != 2)
    err_quit("Usage: a.out <file_name>");
  if ((fpin = fopen(argv[1], "r")) == NULL)
    err_sys("cannot open %s", argv[1]);

  if ((fpout = popen(PAGER, "w")) == NULL)
    err_sys("failed to call popen");

  while (fgets(line, MAXLINE, fpin) != NULL) {
    if (fputs(line, fpout) == EOF)
      err_sys("failed to write into channel");
  }
  if (ferror(fpin))
    err_sys("failed to call fgets");
  if (pclose(fpout) == -1)
    err_sys("failed to call pclose");

  exit(0);
}
