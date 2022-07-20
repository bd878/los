#include "lib.h"

static void my_exit1(void);
static void my_exit2(void);

/*
 * atexit registers exit handlers
 * and calls them in reverse order
 * once main process is exited
 **/
int
main(void)
{
  if (atexit(my_exit2) != 0) {
    printf("failed to register handler my_exit2\n"); 
  }

  if (atexit(my_exit1) != 0) {
    printf("failed to register handler my_exit1\n");
  }
  if (atexit(my_exit1) != 0) {
    printf("failed to register handler my_exit1 (again)\n");
  }

  printf("main function finished\n");
  return(0);
}

static void
my_exit1(void)
{
  printf("first exit handler\n");
}

static void
my_exit2(void)
{
  printf("second exit handler\n");
}
