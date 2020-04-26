#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>

main (int argc, char **argv) {
    char buffer[80];
    struct sockaddr_in     servaddr; 
  
    struct addrinfo hints;
    struct addrinfo *res;
    getaddrinfo (argv[1], argv[2], &hints, &res);

    int sockfd = sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr)); 
    
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[2])); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

    int n;
    socklen_t len; 
      
    sendto(sockfd, (const char *)argv[3], strlen(argv[3]), 
        0, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 

    n = recvfrom(sockfd, (char *)buffer, 80, 0, 
	(struct sockaddr *) &servaddr, &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
} 