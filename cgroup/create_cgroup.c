#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

void free_buffer(char **buffer)
{
	printf("free buffer\n");
	free(*buffer);
}

void cleanup_file(FILE **fp)
{
	printf("close file\n");
	fclose(*fp);
}

int main()
{
	size_t len = 80;
	ssize_t readn;
	char *buffer __attribute__((__cleanup__(free_buffer))) = malloc(len);
	FILE *fp __attribute__((__cleanup__(cleanup_file)));
	unsigned char pathname[20];

	if (sprintf(pathname, "/proc/%d/cgroup", getpid()) < 0)
		err(EXIT_FAILURE, "sprintf");

	fp = fopen(pathname, "r");
	if (fp == NULL) 
		err(EXIT_FAILURE, "fopen");

	readn = getline(&buffer, &len, fp);
	if (readn < 0)
		err(EXIT_FAILURE, "getline");
	printf("readn=%ld, len=%ld\n", readn, len);

	printf("%s\n", buffer);

	return 0;
}
