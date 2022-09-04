#include "lib.h"

#define TOK_ADD 5

void do_line(char *);
void cmd_add(void);
int get_token(void);

/*
 * Execute defined
 * commands on each line
 **/
int
main(void)
{
  char line[MAXLINE];
  while (fgets(line, MAXLINE, stdin) != NULL)
    do_line(line);
  exit(0);
}

char *tok_ptr;

void
do_line(char *ptr)
{
  int cmd;

  tok_ptr = ptr;
  while ((cmd = get_token()) > 0) {
    switch (cmd) { /* every cmd has own case branch */
      case TOK_ADD:
        cmd_add();
        break;
    }
  }
}

void
cmd_add(void)
{
  int token;
  token = get_token();
  /* ... */
}

int
get_token(void)
{
  /* derive the lexem from the string */
  /* that tok_ptr is pointed on */
  return 0;
}
