#include "lib.h"

int
main(void)
{
  int n, int1, int2;
  char line[MAXLINE];

  while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0) {
    line[n] = 0; /* terminate a string by \0 */
    if (sscanf(line, "%d%d", &int1, &int2) == 2) {
      sprintf(line, "%d\n", int1 + int2);
      n = strlen(line);
      if (write(STDOUT_FILENO, line, n) != n)
        err_sys("failed to call write");
    } else {
      if (write(STDOUT_FILENO, "faulty args\n", 19) != 19)
        err_sys("failed to call write");
    }
  }
  exit(0);
}