/*
 * unlinkfail.c - child process opens file, parent process
 * failes to unlink it; child process terminates,
 * parent proc unlinks file successfully
 */
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

const char filename[] = "file.txt";
const char filename2[] = "file2.txt";

void err_exit(const char *fmt, ...);

int main() {
  pid_t pid;
  int fd, fd2;
  int var = 5;

  printf("sizeof(int): %ld\n", sizeof(int));

  if ((fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666)) < 0) {
    perror("open");
    err_exit("failed to open file\n");
  }

  if ((fd2 = open(filename2, O_RDWR | O_CREAT | O_APPEND, 0666)) < 0) {
    perror("open");
    err_exit("failed to open file 2\n");
  }

  printf("fd2: %d\n", fd2);

  if (close(fd2) < 0) {
    perror("close");
    err_exit("failed to close file 2\n");
  }

  printf("opened file with fd: %d\n", fd);

  if ((pid = fork()) < 0) {
    err_exit("failed to fork");
  } else if (pid == 0) { /* child */
    printf("child fd: %d\n", fd);

    if ((fd2 = open(filename2, O_RDWR | O_APPEND)) < 0) {
      perror("open");
      err_exit("failed to open file 2 in child proc\n");
    }

    var++;
    printf("child var = %d\n", var);
    printf("child &var = %p\n", &var);
    printf("child &fd2: %p\n", &fd2);
    printf("child fd2: %d\n", fd2);
    exit(0);
  }

  printf("parent fd: %d\n", fd);

  if ((fd2 = open(filename2, O_RDWR | O_APPEND)) < 0) {
    perror("open");
    err_exit("failed to open file 2 in parent proc\n");
  }

  printf("parent var = %d\n", var);
  printf("parent &var = %p\n", &var);
  printf("parent &fd2: %p\n", &fd2);
  printf("parent fd2: %d\n", fd2);
  return 0;
}

void err_exit(const char *fmt, ...)
{
  va_list  ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
  fflush(NULL);
  exit(1);
}
