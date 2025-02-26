#include <err.h>
#include <stdio.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>

#define STRLITERALLEN(x) (sizeof(""x""))
#define BUF_MAX 1024

ssize_t read_nointr(int fd, void *buf, size_t count);
ssize_t write_nointr(int fd, const void *buf, size_t count);

int main()
{
	int sock;
	int res;
	int n;
	struct ifreq ethreq;
	char buf[BUF_MAX];

	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	if (sock < 0)
		err(EXIT_FAILURE, "socket");

	res = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, "enp7s0", STRLITERALLEN("enp7s0") + 1);
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

	// tcpdump tcp -dd
	struct sock_filter BPF_code[] = {
		{ 0x28, 0, 0, 0x0000000c },
		{ 0x15, 0, 2, 0x00000800 },
		{ 0x30, 0, 0, 0x00000017 },
		{ 0x15, 6, 7, 0x00000006 },
		{ 0x15, 0, 6, 0x000086dd },
		{ 0x30, 0, 0, 0x00000014 },
		{ 0x15, 3, 0, 0x00000006 },
		{ 0x15, 0, 3, 0x0000002c },
		{ 0x30, 0, 0, 0x00000036 },
		{ 0x15, 0, 1, 0x00000006 },
		{ 0x6, 0, 0, 0x00040000 },
		{ 0x6, 0, 0, 0x00000000 }
	};

	struct sock_fprog Filter;
	Filter.len = sizeof(BPF_code) / sizeof(BPF_code[0]);
	Filter.filter = BPF_code;

	if (setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &Filter, sizeof(Filter)) < 0)
		err(EXIT_FAILURE, "setsockopt");

	while (1) {
		printf("-------------\n");
		n = read(sock, buf, 1);
		printf("%d bytes read\n", n);
	}

	return 0;
}

ssize_t read_nointr(int fd, void *buf, size_t count)
{
	ssize_t ret;

	do {
		ret = read(fd, buf, count);
		printf("readen\n");
	} while (ret < 0 && errno == EINTR);

	return ret;
}

ssize_t write_nointr(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	do {
		ret = write(fd, buf, count);
		printf("written\n");
	} while (ret < 0 && errno == EINTR);

	return ret;
}
