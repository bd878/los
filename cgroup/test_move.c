#include <err.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *make_str()
{
	char *str;
	int n;
	str = calloc(sizeof(char), 10);
	n = snprintf(str, 10, "%s", "123456789");
	if (n == -1)
		return NULL;
	return str;
}

int main()
{
	char *str;
	str = make_str();
	if (str == NULL)
		err(EXIT_FAILURE, "make_str");
	printf("%s\n", str);
	return 0;	
}
