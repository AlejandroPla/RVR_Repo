#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

int
main (int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo *res;
  char MY_TIME[80];
  
  struct tm* local;
  time_t t = time(NULL);
  local = localtime(&t);

  memset (&hints, 0, sizeof (struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

  if (rc != 0)
  {
    printf("getaddrinfo: %s\n ", gai_strerror (rc));
    return -1;
  }

  int sd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);

  if (bind (sd, res->ai_addr, res->ai_addrlen) != 0)
  {
    printf("bind: ");
    return -1;
  }
  
  freeaddrinfo (res);

  char buffer[80];
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  struct sockaddr client_addr;
  socklen_t client_len = sizeof (struct sockaddr);

  while (true) {
		ssize_t bytes = recvfrom (sd, buffer, 79 * sizeof (char), 0, &client_addr, &client_len);

		if (bytes == -1) {
			printf("recvfrom: ");
			return -1;
		}

		getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		if (buffer[0] == 't') {
			local = localtime(&t);
			ssize_t s = strftime(MY_TIME, sizeof(MY_TIME), "%H:%M:%S %p", local);
			sendto(sd, MY_TIME, sizeof(char) * s, 0, &client_addr, client_len); 
			printf("%d bytes de %s:%s\n", bytes, host, service);
		} else
		if (buffer[0] == 'd') {
			local = localtime(&t);
			ssize_t s = strftime(MY_TIME, sizeof(MY_TIME), "%Y-%m-%d", local);
			sendto(sd, MY_TIME, sizeof(char) * s, 0, &client_addr, client_len);
			printf("%d bytes de %s:%s\n", bytes, host, service);
		} else 
		if (buffer[0] == 'q') {
			printf("Saliendo...\n");
			shutdown(sd, 2);
			return 0;
		} else {
			printf("Comando no soportado %s\n", buffer[0]);
		}
	}
	
  return 0;
}