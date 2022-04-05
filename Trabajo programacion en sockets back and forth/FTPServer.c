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
	//Our own socket
	socket_t comSocket;
	//The socket of the connecting client.
	their_info comTheirInfo;
	//Poll info for receiving and sending data without blocking. It's declared as a list because it's what "poll()" takes.
	struct pollfd pollInfo[1];

	char inputBuffer[MAX_BUFF_SIZE];
	char outputBuffer[MAX_BUFF_SIZE];
	int numbytes;
	int pollRes;

	//Create connection.
	displayError(initSocket(&comSocket, NULL), 0);
	displayError(bindSocket(&comSocket), 0);
	displayError(listenSocket(&comSocket), 0);
	displayError(acceptSocket(&comSocket, &comTheirInfo), 0);

	destroySocket(&comSocket);

	//Configure the struct, it will inform when there's data to be red or there's the possibility to send without blocking. 
	pollInfo[0].fd = comTheirInfo.theirFd;
	pollInfo[0].events = POLLIN | POLLOUT;
	
	send(comTheirInfo.theirFd, "220 srvFtp version 1.0\r\n", 24, 0);

	//Main server loop
	while (1) {
		//This means poll using the "pollInfo" settings every 10 milliseconds.
		pollRes = poll(pollInfo, 1, -1);

		//Check that there's something to poll.
		if (pollRes != (-1) && pollRes != 0) {
			//There's data ready to be red.
			if (pollInfo[0].revents & POLLIN) {
				numbytes = receiveSocket(comTheirInfo.theirFd, outputBuffer, MAX_BUFF_SIZE);

				printf("> %s\n", outputBuffer);
			}

			//Data can be sent without blocking.
			if (pollInfo[0].revents & POLLOUT) {
				printf(": ");
				fread(inputBuffer, sizeof(char), MAX_BUFF_SIZE, stdin);
        		send(comTheirInfo.theirFd, inputBuffer, strlen(inputBuffer) * sizeof(char), 0);
			}
		}
	}

	destroyTheirSocket(&comTheirInfo);

	return 0;
}