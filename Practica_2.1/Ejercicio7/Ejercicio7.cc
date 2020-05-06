#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <iostream>

class Responder {
private:
	int sd_client;

public:
	Responder(int socket) { sd_client = socket; }

	void responder() {
		ssize_t bytes;

		bool exit = false;
		while (!exit) {
			char buffer[80];

			bytes = recv(sd_client, (void *) buffer, sizeof (char) * 79, 0);

			if (bytes <= 0) { 
				printf ("CONEXION TERMINADA\n"); 
				exit = true;
			} else {
				send(sd_client, (void *) buffer, bytes, 0);
			}
		}
	}
};

int main (int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *res;
	std::vector<std::thread> pool;

	memset (&hints, 0, sizeof (struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

	if (rc != 0) {
		printf ("getaddrinfo: %s \n", gai_strerror (rc));
		return -1;
	}

	int sd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);

	if (bind (sd, res->ai_addr, res->ai_addrlen) != 0) {
		printf ("bind: \n");
		return -1;
	}

	freeaddrinfo (res);

	// ---------------------------------------------------------------------- //
	// PUBLICAR EL SERVIDOR (LISTEN) //
	// ---------------------------------------------------------------------- //
	listen (sd, 16);

	// ---------------------------------------------------------------------- //
	// GESTION DE LAS CONEXIONES AL SERVIDOR //
	// ---------------------------------------------------------------------- //
	while (true) {
		struct sockaddr client_addr;
		socklen_t client_len = sizeof (struct sockaddr);

		char host[NI_MAXHOST];
		char service[NI_MAXSERV];

		int sd_client = accept (sd, &client_addr, &client_len);

		getnameinfo (&client_addr, client_len, host, NI_MAXHOST, service,
		NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		printf ("CONEXION DESDE IP: %s PUERTO: %s \n", host, service);

		Responder r(sd_client);
		pool.push_back(std::thread(&Responder::responder, &r));
		pool.back().join();

	}

	return 0;
}