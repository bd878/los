/**
 * resolve_host_name.c - resolves given domain name,
 * returns IPv4 addr
 */
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
  int s;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  struct sockaddr_in *aiaddr;
  const char *addr;
  char abuf[INET_ADDRSTRLEN];

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <domain.name>\n", argv[0]);
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;

  s = getaddrinfo(argv[1], NULL, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    if (rp->ai_family == AF_INET) {
      aiaddr = (struct sockaddr_in *) rp->ai_addr;
      addr = inet_ntop(AF_INET, &aiaddr->sin_addr, abuf, INET_ADDRSTRLEN);
      printf("addr: %s\n", addr ? addr : "<unknown>");
      printf("port: %d\n", ntohs(aiaddr->sin_port));
    }
  }

  freeaddrinfo(result);
  exit(0);
}

