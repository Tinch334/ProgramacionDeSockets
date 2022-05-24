#ifndef __FTPC_H__
#define __FTPC_H__


#include <stddef.h>


/*
    The command actually used to make FTP commands,. The "command" is the four letter FTP command, "msgContents" what the message has to contain after the code, "finishedCommand" where the command will be stored.
*/
void makeCommandFromString(const char *command, const char *msgContents, char *finishedCommand);


/*
    Makes the proper FTP client side command with user input. The "command" is the four letter FTP command, "displayMsg" the message to be shown to the user and "finishedCommand" where the command will be stored.
*/
void makeCommand(const char *command, const char *displayMsg, char *finishedCommand);


/*
    Gets the IP and port of a given file descriptor. Both the IP and port are not returned by the function, instead a pointer to the variable where they will be stored is passed.
*/
void getSocketInfo(const int fd, char *ip, int *port);


/*
    Makes the arguments for the PORT command and sends it.
*/
void makePortCommand(const int fd);


#endif /* __FTPC_H__ */