#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include "socketCommon.h"
#include "FTPServer.h"
#include "dictionary.h"

#include <unistd.h>

#define PORT "21" //The port users will be connecting to. Since we are using FTP it's port 21.
#define MAXBUFF 1000

int main(int argc, char *argv[])
{
	socket_t comSocket;
	their_info comTheirInfo;

	//For testing only.
	char recieveBuff[MAX_BUFF_SIZE];
	char sendBuff[MAX_BUFF_SIZE];
	char recievedCode[5];

	int numbytes;

	//Create connection.
	displayError(initSocket(&comSocket, NULL), 0);
	displayError(bindSocket(&comSocket), 0);
	displayError(listenSocket(&comSocket), 0);
	displayError(acceptSocket(&comSocket, &comTheirInfo), 0);

	destroySocket(&comSocket);

	//Send connection message.
	sendSocket(comTheirInfo.theirFd, MSG_220, strlen(MSG_220));
	fprintf(stdout, "Sent star msg\n");

	while(1) {
		//numbytes = receiveSocket(comTheirInfo.theirFd, recieveBuff, MAX_BUFF_SIZE);
		numbytes = recv(comTheirInfo.theirFd, recieveBuff, MAX_BUFF_SIZE, 0);
		//Print the recieved message.
		fprintf(stdout, "%s\n", recieveBuff);
		printf("Hola\n");

		//Get FTP code from the client.
		memcpy(recievedCode, recieveBuff, 4);

		switch (dictLookup(recieveBuff)) {
			case USER_DICT:
				fprintf(stdout, "%s", recieveBuff);
				break;
		}
	}


	return 0;
}