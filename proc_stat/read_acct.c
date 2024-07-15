/**
 * read_acct.c - reads acct file
 * and outputs info to stdout
 */
#include <sys/acct.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define FMT "%s\n"

int main(int argc, char** argv) {
  char* me;
  char* fname;
  FILE* fd;
  struct acct_v3 acdata;

  me = argv[0];

  if (argc != 2) {
    printf("Usage: %s <acct_file>\n", me);
    exit(EXIT_FAILURE);
  }

  fname = argv[1];

  if ((fd = fopen(fname, "r")) == NULL) {
    if (errno == ENOENT) {
      fprintf(stderr, "%s not exists\n", fname);
      exit(EXIT_FAILURE);
    } else {
      perror("fopen");
      exit(EXIT_FAILURE);
    }
  }

  while (fread(&acdata, sizeof(acdata), 1, fd) == 1) {
    printf(FMT,
      acdata.ac_comm
    );
  }

  exit(EXIT_SUCCESS);
}
