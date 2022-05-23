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


#define USERS_FILENAME "Users.txt"


void makeResponse(const char *command_1, const char *command_2, const char *var, char *finishedResponse) {
	strcpy(finishedResponse, command_1);
	strcat(finishedResponse, var);
	strcat(finishedResponse, command_2);
}


int getLineNumber(FILE *file) {
    //Set the file pointer to the beginning of the file for safety.
    fseek(file, 0, SEEK_SET);

    char c;
    //The variable is initially set to one to account for the first line.
    int lines = 1;

    while(!feof(file)){
        c = getc(file);

        if(c == '\n'){
            lines++;
        }
    }

    return lines;
}


int getUsers(FILE *file, int userCount, userAndPass *usersAndPasswords) {
	char lineBuffer[MAX_BUFF_SIZE], userBuffer[MAX_BUFF_SIZE], passwordBuffer[MAX_BUFF_SIZE];
	int cont = 0;

	//Set the file pointer to the beginning of the file for safety.
    fseek(file, 0, SEEK_SET);

	//Read the file line by line
	while (fscanf(file, "%s", lineBuffer) != EOF) {
		//Get username and password.
		strcpy(usersAndPasswords[cont].user, strtok(lineBuffer, ","));
		strcpy(usersAndPasswords[cont].password, strtok(NULL, ","));
		
		cont++;
	}

	return cont;
}


int checkUser(int userCount, userAndPass *usersAndPasswords, const char *givenUser) {
	for (int i = 0; i < userCount; i++) {
		if (strcmp(givenUser, usersAndPasswords[i].user) == 0)
			return i;
	}

	return -1;
	
} 


int checkPassword(int userIndex, userAndPass *usersAndPasswords, const char *givenPassword) {
	//Check if the given password is the same as the user's password.
	if (strcmp(usersAndPasswords[userIndex].password, givenPassword) == 0)
		return 1;
	else
		return (-1);
}


int main(int argc, char *argv[])
{
	//The socket pair used to send commands.
	socket_t comSocket;
	their_info comTheirInfo;
	//The socket pair used to send files.

	FILE *usersFile;

	/*
		Send and receive buffers.
	*/
	//The buffers for sending and receiving data.
	char recieveBuff[MAX_BUFF_SIZE];
	char sendBuff[MAX_BUFF_SIZE];
	//The 4 letter code sent by the user.
	char recievedCode[5];
	//The rest of what was sent by the user.
	char receivedMsg[MAX_BUFF_SIZE];

	/*
		Send and receive buffers.
	*/
	int userCount;
	int userIndex;
	//Is 1 if the user is logged in, (-1) if not.
	int userLogged = (-1);

	/*
		Program variables.
	*/
	int programExit = 0;

	//Get usernames and passwords.
	usersFile = fopen(USERS_FILENAME, "r");
	userCount = getLineNumber(usersFile);
	userAndPass usersAndPasswords[userCount];
	getUsers(usersFile, userCount, usersAndPasswords);
	fclose(usersFile);

	//Create command connection.
	displayError(initSocket(&comSocket, NULL, FTP_PORT_COM), 0);
	displayError(bindSocket(&comSocket), 0);
	displayError(listenSocket(&comSocket), 0);
	displayError(acceptSocket(&comSocket, &comTheirInfo), 0);

	destroySocket(&comSocket);

	//Create data connection.

	//Send connection message. As a side note, when using "strlen" on strings containing the "\r\n" combination add 1 to
	//the given length. This is because said combination is interpreted as a linefeed, the ASCII character 10 and thus
	//counted as only one char.
	sendSocket(comTheirInfo.theirFd, MSG_220, msgstrlen(MSG_220));

	while(!programExit) {
		receiveSocket(comTheirInfo.theirFd, recieveBuff, MAX_BUFF_SIZE);
		
		//Print the received message.
		fprintf(stdout, "%s\n", recieveBuff);		

		//Get FTP code from the client.
		memcpy(recievedCode, recieveBuff, 4);
		//Get the rest of the message and remove the space.
		strcpy(receivedMsg, &recieveBuff[5]);
		//Remove the "\r\n" at the end of the message.
		receivedMsg[strlen(receivedMsg) - 1] = '\0';

		switch (dictLookup(recievedCode)) {
			case USER_DICT:
				userIndex = checkUser(userCount, usersAndPasswords, receivedMsg);

				//Check that the username actually exists.
				if (userIndex != (-1)) {
					makeResponse(MSG_331_1, MSG_331_2, receivedMsg, sendBuff);
					sendSocket(comTheirInfo.theirFd, sendBuff, MAX_BUFF_SIZE);
				}
				else {
					//Send username not found error.
					sendSocket(comTheirInfo.theirFd, MSG_332, msgstrlen(MSG_332));
				}

				break;

			case PASS_DICT:
				userLogged = checkPassword(userIndex, usersAndPasswords, receivedMsg);

				if (userLogged == 1) {
					makeResponse(MSG_230_1, MSG_230_2, usersAndPasswords[userIndex].user, sendBuff);
					sendSocket(comTheirInfo.theirFd, sendBuff, MAX_BUFF_SIZE);
				}	
				else {
					//Send incorrect password error.
					sendSocket(comTheirInfo.theirFd, MSG_530, msgstrlen(MSG_530));
				}

				break;

			case QUIT_DICT:
				//Exit program while loop.
				programExit = 1;

				break;
		}

		//Reset buffers each receive cycle to avoid errors.
		memset(recieveBuff, 0, MAX_BUFF_SIZE);
		memset(sendBuff, 0, MAX_BUFF_SIZE);
	}

	//Free the socket from the client.
	destroyTheirSocket(&comTheirInfo);

	return 0;
}