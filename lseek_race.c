#include "lib.h"

/*
 * Child and parent processes
 * are in race for
 * file inode. The last occupied
 * the file overwrites previous
 * changes
 **/

const char BUF[] = "hello, world!\n";
const int BUF_LEN = sizeof(BUF);

int
main(void)
{
  int fd;
  if ((fd = open("race_write.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
    printf("failed to create race_write.txt file\n");
    exit(1);
  }

  if (write(fd, BUF, BUF_LEN) != BUF_LEN) {
    printf("failed to write\n");
    exit(1);
  }

  exit(0);
}
