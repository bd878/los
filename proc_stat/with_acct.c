/**
 * with_acct.c - starts proc.out and writes
 * its accounting info.
 * Outputs accouting info to stdout
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

#define ACCT_FILE "acct.txt"

int main(int argc, char** argv) {
  char* me;
  char* prog_name;
  pid_t cid;

  if (argc < 2) {
    printf("Usage: %s <.exe> ...args\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  me = argv[0];
  prog_name = argv[1];

  if (open(ACCT_FILE, O_CREAT | O_TRUNC, 644) < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  printf("[%s] acct file name: %s\n", me, ACCT_FILE);

  if (acct(ACCT_FILE) < 0) {
    perror("acct");
    goto failure;
  }

  printf("[%s] exec program: %s\n", me, prog_name);

  if ((cid = fork()) < 0) {
    perror("fork");
    goto failure;
  } else if (cid == 0) {
    if (execl(prog_name, prog_name, argv[2], (char *)0) < 0) {
      perror("execv");
      goto failure;
    }
  }

  if (waitpid(cid, NULL, 0) < 1) {
    fprintf(stderr, "[%s] waitpid failed for pid %d\n", me, cid);
    goto failure;
  }

  printf("[%s] done.\n", me);

  acct(NULL);

  return EXIT_SUCCESS;

failure:
  acct(NULL);
  exit(EXIT_FAILURE);
}
