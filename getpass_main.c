#include "getpass.h"
#include "lib.h"

int
main(void)
{
  char *ptr;
  if ((ptr = getpass("Enter password: ")) == NULL)
    err_sys("failed to call getpass");
  printf("password: %s\n", ptr);

  /* handle password somehow (encrypt?) */

  while (*ptr != 0)
    *ptr++ = 0; /* erase - fullfill with zeros, once not neccessary anymore */
  exit(0);
}
