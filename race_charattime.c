#include "lib.h"

static void charatatime(char *);

int
main(void)
{
  pid_t pid;

  if ((pid = fork()) < 0) {
    printf("failed to call fork\n");
    exit(1);
  } else if (pid == 0) {
    charatatime("from child proc\n");
  } else {
    charatatime("from parent proc\n");
  }
  exit(0);
}

static void
charatatime(char *str)
{
  char *ptr;
  int c;

  /* we switch to unbuffered mode */
  /* to force compiler call write syscall */
  /* as frequent as possible */
  setbuf(stdout, NULL);
  for(ptr = str; (c = *ptr++) != 0; ) {
    putc(c, stdout);
  }
}
