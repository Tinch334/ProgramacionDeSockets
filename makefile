CC=gcc
CFLAGS=-I -gstabs -Wall


FTPClient: FTPClient.c FTPClient.h socketCommon.o
	gcc -o FTPClient FTPClient.c FTPClient.h socketCommon.o


FTPServer: FTPServer.c FTPServer.h socketCommon.o
	gcc -o FTPServer FTPServer.c FTPServer.h socketCommon.o

socketCommon.o: socketCommon.c socketCommon.h
	gcc -c -o socketCommon.o socketCommon.c