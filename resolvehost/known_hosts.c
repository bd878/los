/**
 * known_host.c - prints host names, known to
 * this host
 */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
  struct hostent *ent;
  char** alias_l;
  char** addr_l;
  const char* net_addr;
  char addr_str[INET_ADDRSTRLEN];

  if ((ent = gethostent()) == NULL) {
    perror("gethostent");
    exit(1);
  }

  printf("host name: %s\n", ent->h_name);
  switch (ent->h_addrtype) {
  case AF_INET:
    printf("ipv4 addr\n");
    break;
  case AF_INET6:
    printf("ipv6 addr\n");
    break;
  default:
    printf("unknown addr type\n");
  }

  for (alias_l = ent->h_aliases; *alias_l != NULL; alias_l += 1)
    printf("host alias: %s\n", *alias_l);

  if (ent->h_addrtype == AF_INET) {
    for (addr_l = ent->h_addr_list; *addr_l != NULL; addr_l += 1) {
      net_addr = inet_ntop(AF_INET, *addr_l, addr_str, INET_ADDRSTRLEN);
      printf("net addr: %s\n", net_addr ? net_addr : "<unknown>");
    }
  }

  return 0;
}
