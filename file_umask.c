#include "lib.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

/*
 * Changes file creation mask
 * for two files. 
 * umask disables file permission bits
 * for the current process
 * umask is inherited by child processes
 * umask is usefull when lib call
 * does not accept file permissions flags,
 * though caller wants to
 * impose some restrictions
 **/
int
main(void)
{
  umask(0); /* mask is reset, all permissions are allowed to be set */
  if (creat("foo", RWRWRW) < 0) { /* umask is 0, so RWRWRW permissions are set */
    printf("failed to call creat for foo file\n");
    exit(1);
  }

  umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /* umask 066, so 066 permission bits unabled to be set */
  if (creat("bar", RWRWRW) < 0) {
    printf("failed to call creat for bar file\n");
    exit(1);
  }

  exit(0);
}
