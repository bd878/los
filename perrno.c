#include <errno.h>
#include "lib.h"

/*
* Prints error message, received by strerror,
* and error message from errno variable
*
*/

int
main(int argc, char *argv[])
{
  fprintf(stderr, "EACCESS: %s\n", strerror(EACCES));
  errno = ENOENT;
  perror(argv[0]);
  exit(0);
}
