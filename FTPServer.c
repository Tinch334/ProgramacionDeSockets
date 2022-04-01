#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "22" //The port users will be connecting to
#define MAXBUFF 1000

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage theirAddr;
	socklen_t theirLen;
	char buf[MAXBUFF];
	char s[1000];
	int status;
	int socketfd; //Identifies the socket once it's created
	int numbytes;

	//We set the preferences for the connection.
	memset(&hints, 0, sizeof(hints)); //Sets all camps in struct to null
	hints.ai_family = AF_INET; // Use IPv4
	hints.ai_socktype = SOCK_DGRAM; //Use UDP datagrams
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_UDP; //Set UDP protocol

	//We try to establish the connection.
	if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
		fprintf(stderr, "No se pudo establecer el servidor. Motivo: %s\n", gai_strerror(status));
		return 1;
	}

	for(p = res; p != NULL; p = p->ai_next) {
		if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == (-1)) {
			perror("listener ready");
			continue;
		}

		if (bind(socketfd, p->ai_addr, p->ai_addrlen) == (-1)) {
			close(socketfd);
			perror("bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Failed to bind socket");
	}

	freeaddrinfo(res);//Frees the struct used to establish the connection.

	if ((numbytes = recvfrom(socketfd, buf, MAXBUFF - 1, 0, (struct sockaddr *)&theirAddr, &theirLen)) != (-1)) {
		perror("recvfrom");
		exit(1);
	}

	//int a = 30 / (5 - 5);	

	fprintf(stderr, "Recieved packet from: %s\n", inet_ntop(theirAddr.ss_family, get_in_addr((struct sockaddr *)&theirAddr), s, sizeof(s)));
	
	buf[numbytes] = "\0";

	fprintf(stderr, "listener: packet contains \"%s\"\n", buf);

	close(socketfd);

	return 0;
}