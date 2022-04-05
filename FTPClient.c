
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "socketCommon.h"


int main(int argc, char *argv[])
{
	socket_t comSocket;

	char buff[MAX_BUFF_SIZE];
	int serverFd;
	int numbytes;

	displayError(initSocket(&comSocket, argv[1]), 1);
	displayError((connectSocket(&comSocket)), 1);

	send(comSocket.fd, "Hello, world!", 13, 0);

	return 0;
}