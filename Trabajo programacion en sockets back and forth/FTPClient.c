#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include "socketCommon.h"
#include "FTPClient.h"


int main(int argc, char *argv[])
{
	socket_t comSocket;

	//Poll info for receiving and sending data without blocking. It's declared as a list because it's what "poll()" takes.
	struct pollfd pollInfo[1];

	char inputBuffer[MAX_BUFF_SIZE];
	char outputBuffer[MAX_BUFF_SIZE];

	int serverFd;
	int numbytes;
	int pollRes;

	displayError(initSocket(&comSocket, argv[1]), 1);
	displayError((connectSocket(&comSocket)), 1);

	while (1) {
		//This means poll using the "pollInfo" settings every 10 milliseconds.
		pollRes = poll(pollInfo, 1, -1);

		//Check that there's something to poll.
		if (pollRes != (-1) && pollRes != 0) {
			//There's data ready to be red.
			if (pollInfo[0].revents & POLLIN) {
				numbytes = receiveSocket(comSocket.fd, outputBuffer, MAX_BUFF_SIZE);

				printf("> %s\n", outputBuffer);
			}

			//Data can be sent without blocking.
			if (pollInfo[0].revents & POLLOUT) {
				printf(": ");
				fread(inputBuffer, sizeof(char), MAX_BUFF_SIZE, stdin);
        		send(comSocket.fd, inputBuffer, strlen(inputBuffer) * sizeof(char), 0);
			}
		}
	}

	destroySocket(&comSocket);

	return 0;
}