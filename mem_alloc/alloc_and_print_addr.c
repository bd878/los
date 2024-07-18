/*
 * alloc_and_print_addr.c - allocates dynamic memory
 * and prints it's address
 */
#include <stdlib.h>
#include <stdio.h>

static const size_t large_size = 1000000;
static const size_t small_size = 10;

void make_memory(void*, size_t);

int main()
{
  void *addr1;
  void *addr2;

  printf("realloc changes memory addr\n");
  make_memory(addr1, large_size);

  printf("realloc keeps memory addr still\n");
  make_memory(addr2, small_size);

  return 0;
}

void make_memory(void *addr, size_t size)
{
  addr = malloc(size);
  printf("addr: %d\n", addr);

  addr = realloc(addr, size*2);
  printf("realloc addr: %d\n", addr);
}
