#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <elf.h>

/*
 * This simple program prints
 * an ELF header of the file,
 * which it receives from first arg
 */

#if defined(_LP64)
#define ElfW(type) Elf64_ ## type
#else
#define ElfW(type) Elf32_ ## type
#endif

void close_file(FILE **fp)
{
	if (*fp)
		fclose(*fp);
}

#define __do_close \
	__attribute__((__cleanup__(close_file)))

void dump_struct(const void *ptr, size_t n);

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <any_file.elf>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	ElfW(Ehdr) hdr;

	size_t ret;
	__do_close FILE *fp;
	const char *fname = argv[1];

	fp = fopen(fname, "rb");
	if (fp == NULL)
		err(EXIT_FAILURE, "fopen");

	printf("sizeof hdr: %ld\n", sizeof(hdr));

	ret = fread(&hdr, sizeof(hdr), 1, fp);
	printf("read %ld Elf64_Ehdr structs\n", ret);
	if (ret != 1)
		err(EXIT_FAILURE, "fread");

	dump_struct(&hdr, sizeof(hdr));

	return 0;
}

void dump_struct(const void *ptr, size_t n)
{
	for (size_t i = 0; i < n; i++)
		printf("%02x", ((char *)ptr)[i]);
	printf("\n");
}
