#include "lib.h"
#include "pty_fork.h"
#include "pty_open.h"
#include <sys/termios.h>

#ifdef LINUX
#define OPTSTR "+d:einv"
#else
#define OPTSTR "d:einv"
#endif

static void set_noecho(int);
void do_driver(char *);
void loop(int, int);

int
main(int argc, char *argv[])
{
  int fdm, c, ignoreeof, interactive, noecho, verbose;
  pid_t pid;
  char *driver;
  char slave_name[20];
  struct termios orig_termios;
  struct winsize size;

  interactive = isatty(STDIN_FILENO);
  ignoreeof = 0;
  noecho = 0;
  verbose = 0;
  driver = NULL;

  opterr = 0;
  while ((c = getopt(argc, argv, OPTSTR)) != EOF) {
    switch (c) {
    case 'd': /* stdin/stdout driver */
      driver = optarg;
      break;
    case 'e': /* turn off slave pty echo output */
      noecho = 1;
      break;
    case 'i': /* ignore stdin EOF */
      ignoreeof = 1;
      break;
    case 'n': /* non-interactive mode */
      interactive = 0;
      break;
    case 'v': /* output extended messages */
      verbose = 1;
      break;
    case '?':
      err_quit("missed option: -%c", optopt);
    }
  }

  if (optind >= argc)
    err_quit("usage: "
      "pty [ -d driver -einv ] program [ arg ... ]");

  if (interactive) { /* current termios and winsize */
    if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
      err_sys("failed to call tcgetattr for stdin");
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &size) < 0)
      err_sys("failed to exec TIOCGWINSZ");
    pid = pty_fork(&fdm, slave_name, sizeof(slave_name),
      &orig_termios, &size);
  } else {
    pid = pty_fork(&fdm, slave_name, sizeof(slave_name),
      NULL, NULL);
  }

  if (pid < 0) {
    err_sys("failed to call fork");
  } else if (pid == 0) { /* child proc */
    if (noecho)
      set_noecho(STDIN_FILENO);

    if (execvp(argv[optind], &argv[optind]) < 0)
      err_sys("failed to start: %s", argv[optind]);
  }

  if (verbose) {
    fprintf(stderr, "slave name = %s\n", slave_name);
    if (driver != NULL)
      fprintf(stderr, "driver = %s\n", driver);
  }

  if (interactive && driver == NULL) {
    if (tty_raw(STDIN_FILENO) < 0) /* set tty to opaque mode */
      err_sys("failed to call tty_raw");
    if (atexit(tty_atexit) < 0) /* reset settings on exit */
      err_sys("failed to call atexit");
  }

  if (driver)
    do_driver(driver); /* shift our stdin/stdout */

  loop(fdm, ignoreeof); /* copy stdin -> ptym, ptym -> stdout */
  exit(0);
}

static void
set_noecho(int fd) /* turn off echo-output for slave pty */
{
  struct termios stermios;

  if (tcgetattr(fd, &stermios) < 0)
    err_sys("failed to call tcgetattr");

  stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

  stermios.c_oflag &= ~(ONLCR);

  if (tcsetattr(fd, TCSANOW, &stermios) < 0)
    err_sys("failed to call tcsetattr");
}