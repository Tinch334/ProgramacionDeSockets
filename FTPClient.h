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



#endif /* __FTPC_H__ */