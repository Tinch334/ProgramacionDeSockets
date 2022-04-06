#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include "socketCommon.h"
#include "FTPServer.h"

#define PORT "21" //The port users will be connecting to. Since we are using FTP it's port 21.
#define MAXBUFF 1000


int main(int argc, char *argv[])
{
	socket_t comSocket;
	their_info comTheirInfo;

	//For testing only.
	char buff[MAX_BUFF_SIZE];
	int numbytes;

	//Create connection.
	displayError(initSocket(&comSocket, NULL), 0);
	displayError(bindSocket(&comSocket), 0);
	displayError(listenSocket(&comSocket), 0);
	displayError(acceptSocket(&comSocket, &comTheirInfo), 0);

	destroySocket(&comSocket);

	numbytes = receiveSocket(comTheirInfo.theirFd, buff, MAX_BUFF_SIZE);

	printf("Message %s\n", buff);

	return 0;
}