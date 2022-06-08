#include "lib.h"

/*
* Print current process id
*
*/

int
main(void)
{
  printf("current process id %ld\n", (long)getpid());
  exit(0);
}
