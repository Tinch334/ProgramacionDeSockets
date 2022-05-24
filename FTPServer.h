#ifndef __FTPS_H__
#define __FTPS_H__

#include <stddef.h>
#include <stdio.h>
#include "socketCommon.h"


typedef struct {
    char user[MAX_BUFF_SIZE];
    char password[MAX_BUFF_SIZE];
} userAndPass;


/*
    Creates a proper response for FTP requests by the user. Both commands are used for the first and second part of the command, like this: "command_1 <var> command_2".
*/
void makeResponse(const char *command_1, const char *command_2, const char *var, char *finishedResponse);


/*
    Gets the number of lines in the given file.
*/
int getLineNumber(FILE *file);


/*
    Gets the users and passwords from the given file.
*/
int getUsers(FILE *file, int userCount, userAndPass *usersAndPasswords);


/*
    Checks to see if the given user exists.
*/
int checkUser(int userCount, userAndPass *usersAndPasswords, const char *givenUser);


/*
    Checks to see if the given password matches the current user.
*/
int checkPassword(int userIndex, userAndPass *usersAndPasswords, const char *givenPassword);

/*
    Checks to see if the given password matches the current user.
*/
void getPortCommandInfo(char *msg, char *ip, int *port);


#endif /* __FTPS_H__ */