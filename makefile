CC=gcc
CFLAGS=-I -gstabs -Wall

all: socketCommon dictionary FTPClient FTPServer

FTPClient: FTPClient.c FTPClient.h socketCommon.o
	gcc -o FTPClient FTPClient.c FTPClient.h socketCommon.o


FTPServer: FTPServer.c FTPServer.h socketCommon.o
	gcc -o FTPServer FTPServer.c FTPServer.h socketCommon.o dictionary.o

socketCommon: socketCommon.c socketCommon.h
	gcc -c -o socketCommon.o socketCommon.c

dictionary: dictionary.c dictionary.h
	gcc -c -o dictionary.o dictionary.c