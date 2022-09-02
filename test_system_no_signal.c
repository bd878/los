#include "lib.h"
#include "system_no_signal.h"
#include <sys/wait.h>

int
main(void)
{
  int status;

  if ((status = system_no_signal("date")) < 0)
    err_sys("failed to call system_no_signal()");

  pr_exit(status);

  if ((status = system("no_such_command")) < 0)
    err_sys("failed to call system_no_signal()");

  pr_exit(status);

  if ((status = system("who; exit 44")) < 0)
    err_sys("failed to call system_no_signal()");

  pr_exit(status);

  exit(0);
}
