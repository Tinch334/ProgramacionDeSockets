#ifndef __FTPC_H__
#define __FTPC_H__


#include <stddef.h>


/*
    Makes the proper FTP client side command. The "command" is the four leter FTP command, "displayMsg" the message to be shown to the user and "finishedCommand" where the command will be stored.
*/
void makeCommand(const char *command, const char *displayMsg, char *finishedCommand);


#endif /* __FTPC_H__ */