#ifndef __FTPS_H__
#define __FTPS_H__

#include <stddef.h>
#include <stdio.h>
#include "socketCommon.h"


#define msgstrlen(str) (strlen(str) + 1)

//Message 220, salute message.
#define MSG_220 "220 srvFTP version 1.0\r\n"
//Message 322, need account.
#define MSG_332 "332 Need account for login\r\n"
//Message 331, password message.
#define MSG_331_1 "331 Password required for "
#define MSG_331_2 "\r\n"
//Message 230, user message.
#define MSG_230_1 "230 User "
#define MSG_230_2 " logged in\r\n"
//Message 530, login incorrect message.
#define MSG_530 "530 Login incorrect\r\n"
//Message 221, goodbye.
#define MSG_221 "221 Goodbye\r\n"
//Message 200, PORT command successful.
#define MSG_200 "200 PORT command successful\r\n"


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