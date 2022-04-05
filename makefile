CC=gcc
CFLAGS=-I -gstabs -Wall


FTPServer: FTPServer.c FTPServer.h socketCommon.o
	gcc -o FTPServer FTPServer.c FTPServer.h socketCommon.o


FTPSClient: talk.c
	gcc -o FTPSClient talk.c


socketCommon.o: socketCommon.c socketCommon.h
	gcc -c -o socketCommon.o socketCommon.c