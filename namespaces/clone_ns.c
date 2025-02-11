#define _GNU_SOURCE
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <limits.h>

/*
 * Clone child on new root
 * and separate data segments.
 * Allocate new data segments with mmap.
 */

#define STACK_SIZE 1024 * 1024 // 1MB

static int child()
{
	printf("cid, uid: %d, %d\n", getpid(), getuid());
	return EXIT_SUCCESS;
}

int main()
{
	char *stack;
	int ret;

	printf("pid: %d, uid: %d\n", getpid(), getuid());

	stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if (stack == MAP_FAILED)
		err(EXIT_FAILURE, "mmap");
	ret = clone(child, stack + STACK_SIZE,
		CLONE_NEWCGROUP | CLONE_NEWUSER | SIGCHLD, NULL);
	if (ret == -1)
		err(EXIT_FAILURE, "clone");

	if (wait(NULL) == -1)
		err(EXIT_FAILURE, "wait");

	printf("child exited\n");
	exit(EXIT_SUCCESS);
}
