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
		fprintf(stdout, ".%s. - .%s. - %d\n", usersAndPasswords[i].user, givenUser, strcmp(givenUser, usersAndPasswords[i].user));
		if (strcmp(givenUser, usersAndPasswords[i].user) == 0)
			return i;
	}

	return -1;
	
} 


int main(int argc, char *argv[])
{
	socket_t comSocket;
	their_info comTheirInfo;

	FILE *usersFile;

	//The buffers for sending and receiving data.
	char recieveBuff[MAX_BUFF_SIZE];
	char sendBuff[MAX_BUFF_SIZE];

	//The 4 letter code sent by the user.
	char recievedCode[5];
	//The rest of what was sent by the user.
	char receivedMsg[MAX_BUFF_SIZE];

	int numbytes;
	int userCount;
	int userIndex;

	//Create connection.
	displayError(initSocket(&comSocket, NULL), 0);
	displayError(bindSocket(&comSocket), 0);
	displayError(listenSocket(&comSocket), 0);
	displayError(acceptSocket(&comSocket, &comTheirInfo), 0);

	destroySocket(&comSocket);

	//Get usernames and passwords.
	usersFile = fopen(USERS_FILENAME, "r");
	userCount = getLineNumber(usersFile);
	userAndPass usersAndPasswords[userCount];
	getUsers(usersFile, userCount, usersAndPasswords);
	fclose(usersFile);

	//Send connection message.
	sendSocket(comTheirInfo.theirFd, MSG_220, strlen(MSG_220));

	while(1) {
		numbytes = receiveSocket(comTheirInfo.theirFd, recieveBuff, MAX_BUFF_SIZE);
		//numbytes = recv(comTheirInfo.theirFd, recieveBuff, MAX_BUFF_SIZE, 0);
		
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
				
				fprintf(stdout, "%d\n", userIndex);

				//Check that the username actually exists.
				if (userIndex != (-1)) {
					makeResponse("331 Password required for ", "\r\n", receivedMsg, sendBuff);
					sendSocket(comTheirInfo.theirFd, sendBuff, MAX_BUFF_SIZE);
				}
				else {
					//Send username not found error.
					sendSocket(comTheirInfo.theirFd, MSG_332, strlen(MSG_332));
				}

				break;

			case PASS_DICT:

		}

		//Reset buffers each recv cycle to avoid errors.
		memset(recieveBuff, 0, MAX_BUFF_SIZE);
		memset(sendBuff, 0, MAX_BUFF_SIZE);
	}

	destroyTheirSocket(&comTheirInfo);

	return 0;
}