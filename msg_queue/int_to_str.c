#include <stdio.h>
#include <math.h>
#include <stdlib.h>

char* int_to_str(int);

int main() {
  char* buf;

  buf = int_to_str(289768);
  printf("%s\n", buf);

  return 0;
}
char* int_to_str(int value) {
  int ndigits;
  char* buf;

  ndigits = (int)log10((double)value)+1;
  printf("number of digits: %d\n", ndigits);
  buf = malloc(sizeof(char)*(ndigits+1)); /* '\0' */

  sprintf(buf, "%d", value);

  return buf;
}
