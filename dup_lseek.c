#include "lib.h"

char buf1[] = "fd1 wrote this message\n";
char buf2[] = "fd2 left a letter\n";

/*
 * Being in the same process,
 * writes to same file through
 * two different duplicated file descriptors.
 * Since different descriptor records
 *   in fd table of same process
 *   point to same single file record,
 *   changes must not be overwritten
 *   (because file offset is stored in 
 *     file record)
 **/
int
main(void)
{
  int fd1, fd2;
  off_t pos1, pos2;

  if ((fd1 = open("dup_file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
    printf("failed to open file\n");
    exit(1);
  }

  if ((fd2 = dup(fd1)) < 0) { /* duplicate file descriptor */
    /* create new entry in fd table */
    /* that point to same record in files table */
    printf("failed to dup fd1\n");
    exit(1);
  }

  if (write(fd1, buf1, sizeof(buf1)-1) != sizeof(buf1)-1) { 
    /* write changes position in file */
    printf("failed to write buf1 through fd1");
    exit(1);
  }

  /* show current file position */
  pos1 = lseek(fd1, 0, SEEK_CUR);
  pos2 = lseek(fd2, 0, SEEK_CUR);

  printf("position in a file, accessed by fd1: %ld\n", pos1);
  printf("position in a file, accessed by fd2: %ld\n", pos2);

  if (write(fd2, buf2, sizeof(buf2)-1) != sizeof(buf2)-1) {
    /* write to the end of file */
    printf("failed to write buf2 through fd2");
    exit(1);
  }

  exit(0);
}
