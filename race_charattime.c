#include "lib.h"

static void charattime(char *);

int
main(void)
{
}

static void
charattime(char *str)
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
