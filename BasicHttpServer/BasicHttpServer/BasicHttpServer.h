#ifndef BASICHTTPSERVER_H
#define BASICHTTPSERVER_H

#include<stdio.h>
#include<WinSock2.h>
#include<Windows.h>
#include<ws2tcpip.h>
#include"ErrorCodes.h"
#include"HandleHTTPRequest.h"
#include"Logger.h"

int InitializeSocket();
int ListenForClientConnection();
DWORD WINAPI HandleClientRequestThread(LPVOID lpParam);

extern int nMaxClientRequestSize;
extern int nThreadPoolSize;
extern char *strConfigFilePath;
extern int nConfiguredPortNumbered;

#endif