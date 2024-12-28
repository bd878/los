/**
 * launch.c - launches given script
 * and measures some statistics (time)
 */
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

/**
 * Derives script args from argv,
 * accomodating enough space for
 * execv call
 */
static char** derive_script_args(int, char *dst[], char *src[]);

/**
 * Print times difference
 */
static void pr_times(clock_t);

/**
 * Execs given script,
 * waits it to finish
 */
static void exec_script(char *, char **);

int main(int argc, char *argv[]) {
  char *script;
  char **args;
  clock_t start, end;
  struct tms tmstart, tmend;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s script ...args\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  script = argv[1];
  args = derive_script_args(argc, args, argv);

  if ((start = times(&tmstart)) < 0)
    fprintf(stderr, "failed to obtain start time: %s\n", strerror(errno));

  exec_script(script, args);

  if ((end = times(&tmend)) < 0)
    fprintf(stderr, "failed to obtain end time: %s\n", strerror(errno));

  printf("===\n");
  pr_times(end - start);

  exit(EXIT_SUCCESS);
}

static void exec_script(char *script, char *args[]) {
  pid_t cid;

  if ((cid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid == 0) { /* child */
    if (execv(script, args) < 0) {
      perror("execl");
      exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
  }
  /* parent */

  if (waitpid(cid, NULL, 0) < 0) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }
}

static char** derive_script_args(int argc, char *args[], char *argv[]) {
  static const int script_args_idx = 1; /* not 2 because 1 is script name, required by execv */

  size_t items_count, str_len;

  items_count = (argc-2 /* a.out + script */)+1+1 /* null ptr + script name */;
  args = malloc(items_count * sizeof(char *));
  if (args == NULL) {
    fprintf(stderr, "failed to malloc %d bytes: %s\n",
      items_count, strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < items_count-1; i++) {
    str_len = sizeof(argv[script_args_idx+i]);
    args[i] = malloc(str_len);
    strncpy(args[i], argv[script_args_idx+i], str_len);
  }
  
  args[items_count-1] = NULL;

  return args;
}

static void pr_times(clock_t diff)
{
  static long clock_ticks = 0;
  if (clock_ticks == 0)
    if ((clock_ticks = sysconf(_SC_CLK_TCK)) < 0) {
      perror("sysconf");
      exit(EXIT_FAILURE);
    }

  printf("real time: %7.9f\n", diff / (double) clock_ticks);
}
