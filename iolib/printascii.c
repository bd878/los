/**
 * printascii.c - print ascii symbol table
 */
#include <stdio.h>

int main() {
  printf("sizeof(char): %ld\n", sizeof(char));
  printf("sizeof(int): %ld\n", sizeof(int));
  for (size_t i = 0; i < 255; i++) {
    printf("%d - %c\n", i, i);
  }
  return 0;
}
