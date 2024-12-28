#include <stdio.h>

/* demo usage of cleanup variable */

void clean_up(int *final_value)
{
	printf("cleaning up\n");
	printf("final value: %d\n", *final_value);
}

int main(int argc, char **argv)
{
	int avar __attribute__ ((__cleanup__(clean_up))) = 1;
	avar = 5;

	return 0;
}
