#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <iostream>

class Responder {
private:
	int sd;

public:
	Responder(int socket) { sd = socket; }

	void responder(char c, struct sockaddr client_addr,
		socklen_t client_len) {
		char MY_TIME[80];
		struct tm* local;
		time_t t = time(NULL);
		local = localtime(&t);

		printf("Thread %d\n", std::this_thread::get_id());

		if (c == 't') {
			local = localtime(&t);
			ssize_t s = strftime(MY_TIME, sizeof(MY_TIME), 
				"%H:%M:%S %p", local);
			sendto(sd, MY_TIME, sizeof(char) * s, 0, &client_addr, 						client_len);
		} else if (c == 'd') {
			local = localtime(&t);
			ssize_t s = strftime(MY_TIME, sizeof(MY_TIME), 
				"%Y-%m-%d", local);
			sendto(sd, MY_TIME, sizeof(char) * s, 0, &client_addr, 						client_len);
		} else {
			printf("Comando no soportado %c", c);
		}
	}
};

int main (int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *res;

	memset (&hints, 0, sizeof (struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

	if (rc != 0) {
		printf("getaddrinfo: %s\n ", gai_strerror (rc));
		return -1;
	}

	int sd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);

	if (bind (sd, res->ai_addr, res->ai_addrlen) != 0) {
		printf("bind: ");
		return -1;
	}

	freeaddrinfo (res);

	char buffer[80];
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	std::vector<std::thread> pool;

	struct sockaddr client_addr;
	socklen_t client_len = sizeof (struct sockaddr);
	
	bool exit = false;
	while (!exit) {
		ssize_t bytes = recvfrom (sd, buffer, 79 * sizeof (char), 0,
			&client_addr, &client_len);

		getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
			NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		printf("%d bytes de %s:%s\n", bytes, host, service);

		char c = buffer[0];
		if (c == 'q') {
			for (auto &t: pool) {
				return 0;
			}
		}

		sleep(3);
		Responder r(sd);
		pool.push_back(std::thread(&Responder::responder, &r, c, 
			client_addr, client_len));
		pool.back().join();
	}

	close(sd);

  return 0;
}