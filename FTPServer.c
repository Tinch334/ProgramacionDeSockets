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
#include "FTPServer.h"


#define PORT "21" //The port users will be connecting to. Since we are using FTP it's port 21.
#define MAXBUFF 1000


int main(int argc, char *argv[])
{
	socket_t comSocket;
	their_info comThInfo;

	//For testing only.
	char buff[1000];
	size_t buffLen = 1000;
	int numbytes;

	//Create conection.
	displayError(initSocket(&comSocket, NULL), 0);
	displayError(bindSocket(&comSocket), 0);
	displayError(listenSocket(&comSocket), 0);
	displayError(acceptSocket(&comSocket, &comThInfo), 0);

	destroySocket(&comSocket);

	//For testing only.
	if ((numbytes = recv(comThInfo.theirFd, buff, buffLen, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buff[numbytes] = '\0';
	printf("Message %s", buff);


}