CC=gcc
CFLAGS=-I -gstabs -Wall

all: FTPClient FTPServer socketCommon

FTPClient: FTPClient.c FTPClient.h socketCommon.o
	gcc -o FTPClient FTPClient.c FTPClient.h socketCommon.o


FTPServer: FTPServer.c FTPServer.h socketCommon.o
	gcc -o FTPServer FTPServer.c FTPServer.h socketCommon.o

socketCommon: socketCommon.c socketCommon.h
	gcc -c -o socketCommon.o socketCommon.c