#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int
main (int argc, char **argv)
{
  char addrstr[100];
  void *ptr;
  struct addrinfo hints;
  struct addrinfo *res;

  memset (&hints, 0, sizeof (struct addrinfo));

  hints.ai_family = AF_UNSPEC;

  int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

  if (rc != 0)
    {
      printf ("getaddrinfo: %s \n", gai_strerror (rc));
      return -1;
    }

  while (res)
    {
      inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

      switch (res->ai_family)
	{
	case AF_INET:
	  ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
	  break;
	case AF_INET6:
	  ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
	  break;
	}
      inet_ntop (res->ai_family, ptr, addrstr, 100);

      char host[NI_MAXHOST];
      char service[NI_MAXSERV];
      getnameinfo (res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service,
		   NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

      printf ("Direccion: %s | Familia: %d | Puerto: %s | Tipo de socket: %d \n", addrstr, res->ai_family, service, res->ai_socktype);


      res = res->ai_next;
    }

  freeaddrinfo (res);

  return 0;
}