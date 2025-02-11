#ifndef __CGROUP_UTILS_H
#define __CGROUP_UTILS_H

#include <sys/types.h>
#include "compiler.h"

#define STRLITERALLEN(x) (sizeof(""x"") - 1)
#define DEFAULT_CGROUP_MOUNTPOINT "/sys/fs/cgroup"

#define iterate_parts(__iterator, __buffer, __separators)			\
	for (char *__p = NULL, *__it = strtok_r(__buffer, __separators, &__p);   \
		(__iterator = __it);						\
		__iterator = __it = strtok_r(NULL, __separators, &__p))

char *read_cgroup(pid_t pid);
char *make_path(const char *first, ...);
char *concat_paths(const char *first, const char *second);
ssize_t write_nointr(int fd, const void *buf, size_t count);
char *make_cgroup_path(const char *cgroup);
int move_to_cgroup(const char *cgroup, pid_t pid);
int create_cgroup(const char *path);

#endif /* __CGROUP_UTILS_H */
