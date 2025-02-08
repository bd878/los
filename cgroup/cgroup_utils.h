#ifndef __CGROUP_UTILS_H
#define __CGROUP_UTILS_H

#include <sys/types.h>

#define STRLITERALLEN(x) (sizeof(""x"") - 1)
#define DEFAULT_CGROUP_MOUNTPOINT "/sys/fs/cgroup"

char *read_self_cgroup();
char *make_path(const char *first, ...);
char *concat_paths(const char *first, const char *second);
ssize_t write_nointr(int fd, const void *buf, size_t count);

#endif /* __CGROUP_UTILS_H */
