#include "lib.h"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

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
    printf("failed to call lseek\n");
    exit(1);
  }

  if (write(fd, buf2, 10) != 10) {
    printf("failed to write buf2\n");
    exit(1);
  }

  exit(0);
}
