#ifndef BASICHTTPSERVER_H
#define BASICHTTPSERVER_H

#include<stdio.h>
#include<WinSock2.h>
#include<Windows.h>
#include<ws2tcpip.h>
#include"ErrorCodes.h"
#include"HandleHTTPRequest.h"

int InitializeSocket();
int ListenForClientConnection();
DWORD WINAPI HandleClientRequestThread(LPVOID lpParam);

#endif