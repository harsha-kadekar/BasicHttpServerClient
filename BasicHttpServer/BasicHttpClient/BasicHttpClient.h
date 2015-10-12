#pragma once
#include<stdio.h>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include<Windows.h>
#include"ErrorCodes.h"

int InitializeSocket();
int DiscussWithServer();
