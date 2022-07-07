#include "lib.h"

char buf[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n";
char child_buf[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n";

int open_race_file(char*);
void lseek_end(int);
void write_to_file(int, char*, size_t);

/*
 * Two processes
 * open same text file
 * and try to write in it.
 * As far as each opened file
 * has its own record in files table,
 * position in a file would differ
 * for these two processes. 
 * As a result, second proc overwrites
 * changes of the first one
 **/
int
main(void)
{
  char filename[] = "race_file.txt";
  int child_fd, fd;
  pid_t child_pid;  

  if ((child_pid = fork()) < 0) {
    printf("failed to fork\n");
    exit(1);
  } else if (child_pid == 0) { /* child process */
    child_fd = open_race_file(filename); /* same fds in two processes does not mean anything */
                                         /* the processes have separate file tables */
    printf("child fd: %d\n", child_fd);
    lseek_end(child_fd);
    write_to_file(child_fd, child_buf, sizeof(child_buf)-1);
  } else { /* parent process */
    fd = open_race_file(filename);
    printf("parent fd: %d\n", fd);
    lseek_end(fd);
    write_to_file(fd, buf, sizeof(buf)-1);
  }

  exit(0);
}

/*
 * Writes to file
 * given char string
 **/
void write_to_file(int fd, char *buf, size_t buf_size) {
  if (write(fd, buf, buf_size) != buf_size) {
    printf("cannot write %s to file %d\n", buf, fd);
    perror("write_to_file");
    exit(1);
  }
}

/*
 * lseeks position in file
 * to the end of file
 **/
void lseek_end(int fd) {
  off_t pos;
  if ((pos = lseek(fd, 0L, SEEK_END)) < 0) {
    printf("cannot lseek %d file\n", fd);
    perror("lseek_end");
    exit(1);
  }
  printf("lseek %d file to pos %ld\n", fd, pos);
}

/*
 * Opens or creates file by provided filename
 * in write only mode and 660 permissions 
 * or throws error.
 * Provided file exists, it truncates
 **/
int open_race_file(char *filename) {
  int fd;

  if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
    printf("failed to create file %s\n", filename);
    perror("open_race_file");
    exit(1);
  }

  return fd;
}
