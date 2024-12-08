#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <string.h>

int main()
{
  char *termi_id;
  FILE *f;
  int fd;

  termi_id = ctermid(NULL);
  if ((f = fopen(termi_id, "r+")) == NULL)
  {
    fprintf(stderr, "failed to open: %s\n", strerror(errno)); 
    exit(EXIT_FAILURE);
  }

  if ((fd = fileno(f)) < 0)
  {
    fprintf(stderr, "failed to obtain fd: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (tcflush(fd, TCIOFLUSH) < 0)
  {
    fprintf(stderr, "failed to flush fd: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  fputs("> \n", f);
  fclose(f);

  return 0;
}
