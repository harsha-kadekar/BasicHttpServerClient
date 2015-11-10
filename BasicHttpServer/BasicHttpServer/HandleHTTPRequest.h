#ifndef HANDLEHTTPREQUEST_H
#define HANDLEHTTPREQUEST_H

#include"ErrorCodes.h"
#include"HttpRequestParsing.h"
#include<stdio.h>

int HandleClientHTTPRequest(char* szRequest, int nRequestSize, SOCKET skClientSocket);
int Handle_ExtMethods(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Get(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Head(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Options(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Delete(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Trace(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Post(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Put(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Connect(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);

#endif
