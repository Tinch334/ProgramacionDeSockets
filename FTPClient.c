#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include "socketCommon.h"
#include "FTPClient.h"


void makeCommand(const char *command, const char *displayMsg, char *finishedCommand) {
	char inputBuffer[MAX_BUFF_SIZE];

	fprintf(stdout, "%s", displayMsg);
	fgets(inputBuffer, MAX_BUFF_SIZE, stdin);

	//Add command code to start of command.
	strcpy(finishedCommand, command);
	//Add inputed string.
	strcat(finishedCommand, " ");
	strcat(finishedCommand, inputBuffer);
}


int main(int argc, char *argv[])
{
	socket_t comSocket;

	char recieveBuff[MAX_BUFF_SIZE];
	char sendBuff[MAX_BUFF_SIZE];
	char recievedCodeChar[4];

	int serverFd;
	int numbytes;
	int recievedCode;

	displayError(initSocket(&comSocket, argv[1]), 1);
	displayError((connectSocket(&comSocket)), 1);

	while(1) {
		numbytes = receiveSocket(comSocket.fd, recieveBuff, MAX_BUFF_SIZE);

		//Print the recieved message.
		fprintf(stdout, "%s", recieveBuff);

		//Get FTP code from the server and turn it into an integer.
		memcpy(recievedCodeChar, recieveBuff, 3);
		recievedCode = atoi(recievedCodeChar);

		switch (recievedCode) {
			case 220:
				makeCommand("USER", "Username: ", sendBuff);
				sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);
				break;

			case 332:
				makeCommand("USER", "Enter a valid username: ", sendBuff);
				sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);

			case 331:
				makeCommand("PASS", "Password: ", sendBuff);
				sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);
				break;
		}

		//Reset buffers each recv cycle to avoid errors.
		memset(recieveBuff, 0, MAX_BUFF_SIZE);
		memset(sendBuff, 0, MAX_BUFF_SIZE);
		
	}

	destroySocket(&comSocket);

	return 0;
}