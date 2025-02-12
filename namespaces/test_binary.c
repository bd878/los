#include <stdio.h>
#include <unistd.h>

/* Test binary program to
 * be executed with clone_ns
 */

int main()
{
	printf("[test_binary] pid=%d, cid=%d\n", getpid(), getuid());
	return 0;
}
