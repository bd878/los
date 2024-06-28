/*
 * lsdir.c - list directory content
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  pid_t ownp = getpid();
  printf("own pid = %d\n", ownp);

  DIR *procdir = opendir("/proc");
  if (procdir == NULL)
  {
    perror("opendir");
    exit(1);
  }
  struct dirent *ent;
  while ((ent = readdir(procdir)) != NULL)
  {
    printf("%d: %s\n", ent->d_ino, ent->d_name);
  }
  printf("exit.");
  return 0;
}
