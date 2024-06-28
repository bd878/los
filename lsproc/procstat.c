/*
 * main.c - list processes from /proc
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
  struct stat buf;

  if (stat("/proc", &buf) < 0)
  {
    perror("stat proc");
    exit(1);
  }
  
  printf("device id: %d\n", buf.st_dev);
  printf("inode number: %d\n", buf.st_ino);
  printf("block size: %d\n", buf.st_blksize);
  printf("blocks allocated: %d\n", buf.st_blocks);

  struct tm *atime = localtime(&buf.st_atim.tv_sec);
  if (atime == NULL)
  {
    perror("st_atim");
    exit(1);
  }

  printf("last access: %s", asctime(atime));
  return 0;
}
