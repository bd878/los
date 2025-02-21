#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define STRLITERALLEN(x) (sizeof(""x""))

int main()
{
	int sock;
	int res;

	sock = socket(AF_PACKET, SOCK_RAW | SOCK_CLOEXEC, 0);
	if (sock < 0)
		err(EXIT_FAILURE, "socket");

	res = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, "enp7s0", STRLITERALLEN("enp7s0"));
	if (res < 0)
		err(EXIT_FAILURE, "secsockopt");

	return 0;
}
