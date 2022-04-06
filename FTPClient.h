#ifndef __FTPC_H__
#define __FTPC_H__

#include <stddef.h>

/*
    Makes the proper FTP client side command.
*/
void makeCommand(const char *command, const char *displayMsg, char *finishedCommand);

#endif /* __FTPC_H__ */