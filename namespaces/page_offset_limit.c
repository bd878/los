#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/resource.h>

/*
 * print PAGE_OFFSET limit,
 * that designates location of split
 * between userspace part and kernel
 * part (kernel address space)
 * in address space of process
 */

int main()
{
	int ret;
	struct rlimit rlim;

	ret = getrlimit(RLIMIT_AS, &rlim);
	if (ret == -1)
		err(EXIT_FAILURE, "getrlimit");

	printf("RLIMIT_AS soft: %ld\n", rlim.rlim_cur);

	ret = getrlimit(RLIMIT_STACK, &rlim);
	if (ret == -1)
		err(EXIT_FAILURE, "getrlimit stack");
	printf("RLIMIT_STACK soft (MB): %ld\n", rlim.rlim_cur / (1024 * 1024));
	return 0;
}
