#include "lib.h"
#include <fcntl.h>

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: a.out <filename>\n");
    exit(1);
  }

  if (access(argv[1], R_OK) < 0)
    printf("access call error for %s file\n", argv[1]);
  else
    printf("read access permitted\n");

  if (open(argv[1], O_RDONLY) < 0)
    printf("open call error for %s file\n", argv[1]);
  else
    printf("open for read successfully\n");

  exit(0);
}
