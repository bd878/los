#include <stddef.h>

#ifndef __dedup_h
#define __dedup_h

static const int MAX_FD = 20;

/*
 * Free fds from a provided
 * array except one
 **/
void close_fds(int[], size_t, int);

/*
 * Naive implementation
 * of dup2 lib function without
 * fcntl use
 **/
int dedup(int fd, int fd2);

#endif
