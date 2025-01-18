#include <stdio.h>
#include <sys/auxv.h>

int main()
{
	printf("%d\n", getauxval(AT_SECURE));
}
