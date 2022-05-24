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
#include "socketCommon.h"


int initSocket(socket_t *sock, const char *host, const char *port) {
	struct addrinfo hints, *p;
	int status;
	//Used by "setsockopt" for configuration.
	int yes = 1;

	//The configuration for the socket.
	memset(&hints, 0, sizeof(hints));//Sets all fields in struct to null.
	hints.ai_family = AF_INET; //Use IPv4.
	hints.ai_protocol = IPPROTO_TCP; //Set TCP protocol.
	hints.ai_socktype = SOCK_STREAM; //Use TCP datagrams.

	//If there's no host use my IP.
	if (!host) {
        hints.ai_flags = AI_PASSIVE;
    }

	//Gets the info about our own host name.
	if ((status = getaddrinfo(host, port, &hints, &sock->info)) == (-1))
		return status;

	//Try to create socket.
	for(p = sock->info; p != NULL; p = p->ai_next) {
		if ((sock->fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			continue;
		}

		//Configure the socket so that other sockets may "bind()" to it. To allow for reconnection in case of server crashes.
		if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == (-1))
			return status;

		break;
	}

	return 0;
}


int bindSocket(socket_t *sock) {
	return bind(sock->fd, sock->info->ai_addr, sock->info->ai_addrlen);
}


int listenSocket(socket_t *sock) {
	return listen(sock->fd, 0);
}


int acceptSocket(socket_t *sock, their_info *thInfo) {
	int tempFd = accept(sock->fd, (struct sockaddr *)&thInfo->theirAddr, &thInfo->theirSize);

	//Check if the incoming connection is valid.
	if (tempFd == (-1)) {
		return tempFd;
	}

	thInfo->theirFd = tempFd;

	//Otherwise return 0.
	return 0;
}


int connectSocket(socket_t *sock) {
	return connect(sock->fd, sock->info->ai_addr, sock->info->ai_addrlen);
}

void destroySocket(socket_t *sock) {
	//Cleans up both the socket and the 
	freeaddrinfo(sock->info);
	close(sock->fd);
}

void destroyTheirSocket(their_info *sock) {
	//Cleans up both the socket and the 
	close(sock->theirFd);
}


//Set type to 0 for server, 1 for the client.
void displayError(const int errorCode, const int type) {
	//Check that there's actually an error.
	if (errorCode != 0)
		fprintf(stderr, "There was a problem with the %s. Reason: %s\n", ((type == 0) ? "server" : "client"), gai_strerror(errorCode));
}


int sendSocket(int fd, const char *buffer, size_t bufSize) {
	int total = 0;
	size_t sent;

	//The while is used because it's possible that some messages may not be sent completely on the first call.
	//The buffer is accessed using the total index to offset it in case that a part of the message has been sent.
	while (total < bufSize) {
		sent += send(fd, &buffer[total], bufSize - total, 0);

		//If we've sent 0 bytes then we've sent all the message.
		if (sent == 0)
			return total;
		else
			total += sent;
	}

	return total;
}


int receiveSocket(const int fd, char *buffer, const size_t bufSize) {
	//This, &buffer[0], is used because we require a pointer to the start of the string.
	return recv(fd, &buffer[0], bufSize, 0);
}


their_info createConnection(const char *host, const char *port, const int type) {
    socket_t connectionSocket;
    their_info theirInfo;

    displayError(initSocket(&connectionSocket, host, port), type);
    displayError(bindSocket(&connectionSocket), type);
    displayError(listenSocket(&connectionSocket), type);
    displayError(acceptSocket(&connectionSocket, &theirInfo), type);

    destroySocket(&connectionSocket);

    return theirInfo;
}