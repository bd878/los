#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>

#define STRLITERALLEN(x) (sizeof(""x""))

int main()
{
	int sock;
	int res;
	struct ifreq ethreq;

	sock = socket(AF_PACKET, SOCK_RAW | SOCK_CLOEXEC, 0);
	if (sock < 0)
		err(EXIT_FAILURE, "socket");

	res = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, "enp7s0", STRLITERALLEN("enp7s0"));
	if (res < 0)
		err(EXIT_FAILURE, "secsockopt");

	strncpy(ethreq.ifr_name, "enp7s0", IF_NAMESIZE);
	res = ioctl(sock, SIOCGIFFLAGS, &ethreq);
	if (res < 0)
		err(EXIT_FAILURE, "ioctl");

	ethreq.ifr_flags |= IFF_PROMISC;
	res = ioctl(sock, SIOCSIFFLAGS, &ethreq);
	if (res < 0)
		err(EXIT_FAILURE, "ioctl");

	return 0;
}
