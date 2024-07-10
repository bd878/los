#include <stdio.h>
#include <string.h>

#ifndef MAXLINE
#define MAXLINE 4096
#endif

static char filename[] = "test.txt";

FILE* new_line_stream(off_t sz);

