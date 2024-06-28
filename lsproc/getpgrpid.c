/*
 * getpgrpid.c - get this process group id
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  pid_t ppid = getpgrp();
  if (ppid < 0)
  {
    perror("getpgrpid");
    exit(1);
  }
  printf("%d\n", ppid);
  return 0;
}
