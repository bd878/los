#include "lib.h"
#define BUFFSIZE 4096

/*
* Copies input from standart input to
* standard out
*
* Usage:
*   echo "some text" | inoutcopy
*
* Uses unbuffered io system calls
* read/write
*
*/

int
main(void)
{
  int n;
  char buf[BUFFSIZE];

  while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, n) != n) {
      printf("stdout write error");
      exit(1);
    }
  }

  if (n < 0)
    printf("stdin read error");

  exit(0);
}
