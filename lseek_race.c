#include "lib.h"

/*
 * Child and parent processes
 * are in race for
 * file inode. The process who occupied
 * the file last overwrites previous
 * changes
 **/
const char BUF[] = "hello, world!\n";
const int BUF_LEN = sizeof(BUF);

int
main(void)
{
  int fd;
  if ((fd = open("race_write.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
    /* O_CREATE - create file if not exists */
    printf("failed to create race_write.txt file\n");
    exit(1);
  }

  if (write(fd, BUF, BUF_LEN) != BUF_LEN) {
    /* Each process supports fd table
     * each record in this table points
     * to record in file table.
     * Every process has its own files table.
     * Each record in file table has current position
     * for this process and a pointer to the file
     * vnode (i-node) that is one for each file
     **/
    printf("failed to write\n");
    exit(1);
  }

  exit(0);
}
