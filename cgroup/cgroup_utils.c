#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "cgroup_utils.h"

/*
 * Reads current cgroup,
 * parses /proc/self/cgroup output
 * on tiles
 */

#define move_ptr(ptr)			\
	({				\
		typeof(ptr) __internal_ptr__ = (ptr); \
		(ptr) = NULL;				\
		__internal_ptr__;			\
	})

void cleanup_buffer(char **buf);

#define __do_free			\
	__attribute__((__cleanup__(cleanup_buffer)))	\

void cleanup_file(FILE **fp)
{
	fclose(*fp);
}

void cleanup_buffer(char **buf)
{
	if (*buf != NULL)
		free(*buf);
}

void *must_realloc(void *orig, size_t sz)
{
	void *ret;

	do {
		ret = realloc(orig, sz);
	} while (!ret);

	return ret;
}

#define BATCH_SIZE 50
void append_line(char **dest, size_t oldlen, char *new, size_t newlen)
{
	size_t full = oldlen + newlen;

	int newbatches = ((full + 1) / BATCH_SIZE) + 1;
	int oldbatches = (oldlen / BATCH_SIZE) + 1;

	if (!dest || newbatches > oldbatches)
		*dest = must_realloc(*dest, newbatches * BATCH_SIZE);

	memcpy(*dest + oldlen, new, newlen + 1);
}

char *read_cgroup(pid_t pid)
{
	FILE *fp __attribute__((__cleanup__(cleanup_file)));
	char *buf __attribute__((__cleanup__(cleanup_buffer))) = NULL;
	char *line __attribute__((__cleanup__(cleanup_buffer))) = NULL;
	int pathn, linelen = 0;
	size_t len, fulllen = 0;
	char pathname[30];
	char separators[] = "\n";
	char *path_part;
	char *path;

	pathn = sprintf(pathname, "/proc/%d/cgroup", pid);
	if (pathn < 0)
		return NULL;

	fp = fopen(pathname, "r");
	if (fp == NULL)
		return NULL;

	while ((linelen = getline(&line, &len, fp)) != -1) {
		append_line(&buf, fulllen, line, linelen);
		fulllen += linelen;
	}

	path = strdup(buf + STRLITERALLEN("0::"));

	iterate_parts(path_part, path, separators) {}

	return move_ptr(path);
}

char *make_path(const char *first, ...)
{
	va_list args;
	char *cur, *dest;
	size_t full_len = strlen(first);
	size_t buf_len;
	size_t cur_len;

	dest = strdup(first);
	cur_len = full_len;

	va_start(args, first);
	while ((cur = va_arg(args, char *)) != NULL) {
		buf_len = strlen(cur);
		if (buf_len == 0)
			continue;

		full_len += buf_len;
		if (cur[0] != '/')
			full_len++;

		dest = must_realloc(dest, full_len + 1);

		if (cur[0] != '/') {
			memcpy(dest + cur_len, "/", 1);
			cur_len += 1;
		}

		memcpy(dest + cur_len, cur, buf_len);
		cur_len += buf_len;
	}
	va_end(args);

	dest[cur_len] = '\0';
	return dest;
}

char *concat_paths(const char *first, const char *second)
{
	__do_free char *result = NULL;
	int ret;
	size_t len;
	int pattern_type = 0;

	len = strlen(first) + strlen(second) + 1;
	if (second[0] != '/') {
		len += 1;
		pattern_type = 1;
	}

	result = calloc(sizeof(char), len);
	if (!result)
		return NULL;

	if (pattern_type == 0)
		ret = snprintf(result, len, "%s%s", first, second);
	else
		ret = snprintf(result, len, "%s/%s", first, second);
	if (ret < 0)
		return NULL;

	return move_ptr(result);
}

ssize_t write_nointr(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	do {
		ret = write(fd, buf, count);
	} while (ret < 0 && errno == EINTR);

	return ret;
}

char *make_cgroup_path(const char *cgroup)
{
	return concat_paths(DEFAULT_CGROUP_MOUNTPOINT, cgroup);
}

int move_to_cgroup(const char *cgroup, pid_t pid)
{
	int fd;
	char pidstr[sizeof(int) + 1];
	size_t pidstr_len;

	fd = open(concat_paths(concat_paths(DEFAULT_CGROUP_MOUNTPOINT, cgroup), "cgroup.procs"), O_WRONLY);
	if (fd == -1)
		return -1;

	if ((pidstr_len = sprintf(pidstr, "%d", pid)) < 0)
		return -1;

	if (write_nointr(fd, pidstr, pidstr_len) == -1)
		return -1;

	return 0;
}
