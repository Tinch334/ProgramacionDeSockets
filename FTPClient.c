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

	makeCommandFromString(command, inputBuffer, finishedCommand);
}


void makeCommandFromString(const char *command, const char *msgContents, char *finishedCommand) {
	//Add command code to start of command.
	strcpy(finishedCommand, command);
	//Add message contents string.
	strcat(finishedCommand, " ");
	strcat(finishedCommand, msgContents);
}


int main(int argc, char *argv[])
{
	socket_t comSocket;

	/*
		Send and receive buffers.
	*/
	char recieveBuff[MAX_BUFF_SIZE];
	char sendBuff[MAX_BUFF_SIZE];
	char recievedCodeChar[4];

	/*
		FTP operation buffers.
	*/
	char operationInputBuff[MAX_BUFF_SIZE];
	char operationBuff[4];

	int serverFd;
	int numbytes;
	int recievedCode;
	int programExit = 0;

	displayError(initSocket(&comSocket, argv[1]), 1);
	displayError((connectSocket(&comSocket)), 1);

	while(!programExit) {
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
				break;

			case 331:
				makeCommand("PASS", "Password: ", sendBuff);
				sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);
				break;

			case 530:
				makeCommand("PASS", "Enter the correct password: ", sendBuff);
				sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);
				break;

			//We have logged in, we now enter the FTP prompt to send commands to the server.
			case 230:
				while (!programExit) {
					//Get user operation.
					fprintf(stdout, "\nftp> ");
					fgets(operationInputBuff, MAX_BUFF_SIZE, stdin);

					//Get actual command
					memcpy(operationBuff, operationInputBuff, 3);
					//Cut first 4 characters off the input to get the arguments to the command.
					memmove(operationInputBuff, operationInputBuff + 4, strlen(operationInputBuff));

					//Process command.
					if (strcmp(operationBuff, "get") == 0) {
						makeCommandFromString("RETR", operationInputBuff, sendBuff);
						sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);
					}

					else if (strcmp(operationBuff, "ext") == 0) {
						//Send quit message to server.
						makeCommandFromString("QUIT", "", sendBuff);
						sendSocket(comSocket.fd, sendBuff, MAX_BUFF_SIZE);

						//Exit both while loops.
						programExit = 1;
					}
					else {
						fprintf(stdout, "\nPlease enter a valid FTP command");
					}
				}

				break;
		}

		//Reset buffers each receive cycle to avoid errors.
		memset(recieveBuff, 0, MAX_BUFF_SIZE);
		memset(sendBuff, 0, MAX_BUFF_SIZE);
		
	}

	//Free our socket.
	destroySocket(&comSocket);

	return 0;
}