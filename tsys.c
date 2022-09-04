#include "lib.h"

int
main(int argc, char *argv[])
{
  int status;

  if (argc < 2)
    err_quit("at least one command argument required");

  if ((status = system(argv[1])) < 0)
    err_sys("failed to call system()");

  pr_exit(status);

  exit(0);
}
