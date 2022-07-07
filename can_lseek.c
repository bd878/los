#include "lib.h"

/*
 * Determines if it is possible
 * to change file position
 * for current input file.
 * Named/unnamed channels, sockets
 * are not seekable.
 * Text files are.
 **/
int
main(void)
{
  /* lseek changes position in a file */
  /* that is accessed through STDIN_FILENO file descriptor */
  if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
    /* lseek may return signed int */
    /* because SEEK_END designates offset from the end of file */
    /* and SEEK_CUR designates offset from current position */
    printf("can not seek\n");
  } else {
    printf("seeked\n");
  }

  exit(0);
}
