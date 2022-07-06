#include "lib.h"

int globvar = 6; /* global variable in */
                 /* initialized data segment (.data) */ 

char buf[] = "write in stdout\n";

/*
 * Forks the process
 * and tries to change variables
 * in different memory segments (on
 *   stack and data segment (.bss section))
 * They should not be mutated since
 * each process has its own copy of
 * stack and data segment (though
 *   code segment is shared still)
 * How many lines will be output to STDIN (
 *   other file)?
 *
 * ./a.out
 * ./a.out > output.txt
 **/
int
main(void)
{
  int var; /* variable on stack */
  pid_t pid;

  var = 88;
  if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) { /* write is syscall, hence not buffered */
    /* -1 is to remove null-terminating byte \0 */
    /* sizeof evaluates on compilation, while strlen is a lib func for runtime use */
    printf("failed to write in STDOUT\n");
    exit(1);
  }

  printf("apres fork call\n"); /* printf bufferes each line when printing to STDOUT */
                               /* printf bufferes output when printing to any other output */
                               /* reset buffer before fork, and get single line copy */
  if ((pid = fork()) < 0) { /* so far it is not defined which process */
                            /* executes first: whether parent of child */
    printf("failed to fork\n");
    exit(1);
  } else if (pid == 0) {
    globvar++; /* change variables in different segments */
    var++; /* from child process */
  } else {
    sleep(2); /* so we pause parent in hope child process pass child */
  }

  printf("pid = %ld, globvar = %d, var = %d\n",
    (long)getpid(), globvar, var);

  exit(0);
}
