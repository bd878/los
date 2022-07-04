#include "lib.h"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

/*
 * Creates file with a hole in the middle
 * Writes a couple of bytes in the beginning,
 * then lseek beyond file size
 * and writes to the end again
 * ./a.out
 * od -c file.hole
 **/
int
main(void)
{
  int fd;

  if ((fd = creat("file.hole", FILE_MODE)) < 0) {
    printf("failed to call creat on \"file.hole\" file\n");
    exit(1);
  }

  if (write(fd, buf1, 10) != 10) {
    printf("failed to write buf1\n");
    exit(1);
  }

  if (lseek(fd, 16634, SEEK_SET) == -1) {
    /* lseek can move file cursor to any position in memory,
     * even beyond the file size
     * Current file position is positive whole number
     * that denotes offset from file start in bytes
     **/
    printf("failed to call lseek\n");
    exit(1);
  }

  if (write(fd, buf2, 10) != 10) {
    printf("failed to write buf2\n");
    exit(1);
  }

  exit(0);
}
