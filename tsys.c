#include "lib.h"

int
main(int argc, char *argv[])
{
  int status;

  if (argc < 2)
    err_quit("at least one command argument required");

  // we call given command (printuids) via system procedure
  // to show that system keeps effective uid (of tsys).
  // It is considered a security breache since
  // gives child processes root permissions.
  // Use fork and exec to switch for raised permissions
  if ((status = system(argv[1])) < 0)
    err_sys("failed to call system()");

  pr_exit(status);

  exit(0);
}
