#include "lib.h"

/*
 * Find out how many CPU ticks
 * is in a second period
 **/
int
main(void)
{
  long val;
  if ((val = sysconf(_SC_CLK_TCK)) < 0) {
    if (errno != 0) {
      if (errno == EINVAL) {
        printf("not supported\n");
      } else {
        printf("failed to call sysconf\n");
        exit(1);
      }
    } else {
      printf("no limits found\n");
    }
  } else {
    printf("%ld ticks per second\n", val);
  }

  exit(0);
}
