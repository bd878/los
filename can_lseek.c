#include "lib.h"

int
main(void)
{
  if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
    printf("can not seek\n");
  else
    printf("seeked\n");

  exit(0);
}
