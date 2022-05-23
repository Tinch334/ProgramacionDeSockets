#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
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


void getSocketInfo(const int fd, char *ip, int *port) {
    //The sockaddr_in stores information about the command connection established with the server.
    struct sockaddr_in connectionInfo;
    //Required for getsockname to work.
    int connectionInfoSize = sizeof(connectionInfo);

    //Get information about the connection to the server.
    getsockname(fd, (struct sockaddr *) &connectionInfo, &connectionInfoSize);

    *port = connectionInfo.sin_port;
    //Convert IP address to printable.
    inet_ntop(AF_INET, &(connectionInfo.sin_addr), ip, MAX_BUFF_SIZE);
}


int main(int argc, char *argv[])
{
    //Socket for command connection to the server.
    socket_t comSocket;
    //Socket for the data connection from the client.
    socket_t dataSocket;
    //Info when the server connects.
    their_info dataTheirInfo;
    //The port used for the command communication.
    int comPort;
    char comIP[MAX_BUFF_SIZE];

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

    int recievedCode;
    int programExit = 0;

    displayError(initSocket(&comSocket, argv[1], FTP_PORT_COM), 1);
    displayError((connectSocket(&comSocket)), 1);

    getSocketInfo(comSocket.fd, comIP, &comPort);

    //For testing only. REMOVE WHEN DONE!!!!
    fprintf(stderr, "Port: %d - IP: %s\n", comPort, comIP);

    while(!programExit) {
        receiveSocket(comSocket.fd, recieveBuff, MAX_BUFF_SIZE);

        //Print the received message.
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
                    //Removes trailing newline from fgets. We could use the simpler approach of
                    //"str[strlen(str) - 1] = '\0'" however this would cause an error on an empty string.
                    operationInputBuff[strcspn(operationInputBuff, "\n")] = 0;

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