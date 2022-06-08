#include "lib.h"
#include <dirent.h>

/*
* Lists files in provided directory
*
* Usage:
*   ls .
* 
*/

int
main(int argc, char *argv[])
{
  DIR *dp;
  struct dirent *dirp;

  if (argc != 2) {
    printf("Usage: ls dir_name");
    exit(1);
  }

  if ((dp = opendir(argv[1])) == NULL) {
    printf("cannot open %s", argv[1]);
    exit(1);
  }

  while ((dirp = readdir(dp)) != NULL) {
    printf("%s\n", dirp->d_name);
  }

  closedir(dp);
  exit(0);
}

