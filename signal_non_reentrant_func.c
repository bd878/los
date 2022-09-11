#include "lib.h"
#include <pwd.h>

static void my_alarm();

/*
 * Non-reentrant function
 * damages memory
 * if called both from main
 * and handler functions.
 * Program might fail
 **/
int
main(void)
{
  struct passwd *ptr;

  signal(SIGALRM, my_alarm);
  alarm(1);
  for ( ; ; ) {
    if ((ptr = getpwnam("sar")) == NULL)
      err_sys("failed to call getpwnam");
    if (strcmp(ptr->pw_name, "sar") != 0)
      printf("returning value damaged!, pw_name = %s\n",
        ptr->pw_name);
  }
}

static void
my_alarm()
{
  struct passwd *rootptr;

  printf("inside signal handler\n");
  if ((rootptr = getpwnam("root")) == NULL)
    err_sys("failed to call getpwnam(root)");
  alarm(1);
}
