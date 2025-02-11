#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "libcgutils-0.1/cgroup_utils.h"

#ifndef O_PATH
#define O_PATH		010000000
#endif

#define NEXT_CGROUP "test"

void print_stat(int fd);

int main()
{
	char *cgroup = read_cgroup(getpid());
	char *next_cgroup;
	char *full_path = NULL;
	char *next_full_path = NULL;
	char pidstr[sizeof(int) + 1];
	size_t pidstr_len;
	int ret, fd, next_fd;

	printf("cgroup: %s\n", cgroup);

	full_path = make_cgroup_path(cgroup);
	if (full_path == NULL)
		err(EXIT_FAILURE, "concat_paths");

	printf("full_path: %s\n", full_path);
	fd = open(full_path, O_CLOEXEC | O_PATH | O_DIRECTORY);
	if (fd == -1)
		err(EXIT_FAILURE, "open");

	ret = mkdirat(fd, NEXT_CGROUP, 0755);
	if (ret == -1 && errno != EEXIST)
		err(EXIT_FAILURE, "mkdirat");

	next_full_path = concat_paths(full_path, NEXT_CGROUP);
	if (next_full_path == NULL)
		err(EXIT_FAILURE, "concat_paths");

	printf("next full path: %s\n", next_full_path);

	next_fd = open(concat_paths(next_full_path, "cgroup.procs"), O_WRONLY);
	if (next_fd == -1)
		err(EXIT_FAILURE, "open");

	print_stat(next_fd);

	if ((pidstr_len = sprintf(pidstr, "%d", getpid())) < 0)
		err(EXIT_FAILURE, "sprintf");

	printf("pidstr_len: %ld, pidstr: %s\n", pidstr_len, pidstr);

	if (write_nointr(next_fd, pidstr, pidstr_len) == -1)
		err(EXIT_FAILURE, "write_nointr");

	next_cgroup = read_cgroup(getpid());
	printf("next cgroup: %s\n", next_cgroup);

	return 0;
}

void print_stat(int fd)
{
	struct stat statbuf;
	int ret;

	ret = fstat(fd, &statbuf);
	if (ret == -1)
		err(EXIT_FAILURE, "fstat");

	printf("mode_t: %d\n", statbuf.st_mode);
}
