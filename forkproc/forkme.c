/*
 * forkme.c - forks, exits parent and become init child
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void printids();

int main()
{
  pid_t child;

  if ((child = fork()) < 0) {
    printf("failed to call fork");
    exit(1);
  } else if (child > 0) {
    printf("child proc %d\n", child);
    printids("parent");
    exit(0);
  }

  printids("child proc");
  return 0;
}

void printids(const char *str)
{
  pid_t myid, ppid;

  myid = getpid();
  ppid = getppid();
  printf("%s: myid: %d, ppid: %d\n", str, myid, ppid);
}
