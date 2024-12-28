#include <stdlib.h>
#include <stdio.h>

#define TMP_FILE "/tmp/tmp.file"

void free_buffer(char **buffer)
{
	printf("freeing buffer\n");
	free(*buffer);
}

void cleanup_file(FILE **fp)
{
	printf("closing file\n");
	fclose(*fp);

	printf("deleting the file\n");
	remove(TMP_FILE);
}

int main(int argc, char **argv)
{
	char *buffer __attribute__((__cleanup__(free_buffer))) = malloc(20);
	FILE *fp __attribute__ ((__cleanup__(cleanup_file)));

	fp = fopen(TMP_FILE, "w+");

	if (fp != NULL)
		fprintf(fp, "%s", "Alinewithnospaces");

	fflush(fp);
	fseek(fp, 0L, SEEK_SET);
	fscanf(fp, "%s", buffer);
	printf("%s\n", buffer);

	return 0;
}
