#include "lib.h"

/*
 * This filter will not work
 * as a soprocess, because
 * standard i/o library buffering
 * input&output. While main proc
 * is waiting result, soproc bufferes output
 */
int
main(void)
{
  int n, int1, int2;
  char line[MAXLINE];

  while (fgets(line, MAXLINE, stdin) != NULL) {
    if (sscanf(line, "%d%d", &int1, &int2) == 2) {
      if (printf("%d\n", int1 + int2) == EOF)
        err_sys("failed to call printfy");
    } else {
      if (printf("wrong args\n") == EOF)
        err_sys("failed to call printf");
    }
  }
  exit(0);
}