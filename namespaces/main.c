#define _GNU_SOURCE
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sched.h>
#include <stdio.h>

/* unshare calling process
 * in new ns namespace.
 * To make it properly,
 * calling process must gain full capabilities
 * (aka CAP_SYS_ADMIN) in calling namespace.
 * That is why we call CLONE_NEWUSER first
 */

#ifndef CLONE_NEWNS
#define CLONE_NEWNS 0x00020000 /* New mount namespace group */
#endif

int main()
{
	int ret;
	printf("pid: %d\n", getpid());
	ret = unshare(CLONE_NEWUSER);
	if (ret == -1)
		err(EXIT_FAILURE, "unshare");

	ret = unshare(CLONE_NEWNS);
	if (ret == -1)
		err(EXIT_FAILURE, "unshare");
	sleep(10000);
}
