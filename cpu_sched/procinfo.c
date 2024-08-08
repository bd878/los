/**
 * procinfo.c - returns info about
 * processors available
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

int main() {
  int proc_avail, proc_config;
  proc_avail = get_nprocs();
  proc_config = get_nprocs_conf();
  printf("[%d] this system has %d procs, and \
configured to have %d procs\n", getpid(), proc_avail, proc_config);
  exit(EXIT_SUCCESS);
}
