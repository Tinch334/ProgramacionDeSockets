#ifndef __SC_H__
#define __SC_H__


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>


#define FTP_PORT_COM "21"
#define MAX_BUFF_SIZE 500

//Message 220, salutte message
#define MSG_220 "220 srvFTP version 1.0\r\n"
//Message 331, password message
#define MSG_331_1 "331 Password required for "
#define MSG_331_2 "\r\n"
//Message 230, user message
#define MSG_230_1 "230 User"
#define MSG_230_2 "230 User logged in\r\n"
//Message 530, login incorrect message
#define MSG_530 "530 Login incorrect\r\n"


typedef struct {
	int fd;
	struct addrinfo *info;
} socket_t;


typedef struct {
	struct sockaddr_storage theirAddr;
	socklen_t theirSize;
	int theirFd;
} their_info;


/*
	Initializes the socket with the given host.
*/
int initSocket(socket_t *socket, const char *host);


/*
	Binds the given socket.
*/
int bindSocket(socket_t *socket);


/*
	Listens on the given socket.
*/
int listenSocket(socket_t *socket);


/*
	Tries to accept the given socket.
*/
int acceptSocket(socket_t *socket, their_info *thInfo);


/*
	Connects the given socket.
*/
int connectSocket(socket_t *socket);


/*
	Destroys the given socket_t;
*/
void destroySocket(socket_t *socket);


/*
	Destroys the given their_info;
*/
void destroyTheirSocket(their_info *sock);


/*
	Displays an error code properly, with it's accompanying message.
*/
void displayError(int errorCode, int type);


/*
	Sends the given buffer to the given socket_t.
*/
int sendSocket(int fd, const char *buffer, size_t bufSize);


/*
	Receives the info from the given socket_t.
*/
int receiveSocket(int fd, char *buffer, size_t bufSize);


#endif /* __SC_H__ */