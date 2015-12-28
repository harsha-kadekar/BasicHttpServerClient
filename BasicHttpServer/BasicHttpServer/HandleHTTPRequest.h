#ifndef HANDLEHTTPREQUEST_H
#define HANDLEHTTPREQUEST_H

#include"ErrorCodes.h"
#include"HttpRequestParsing.h"
#include"SupportingDataStructures.h"
#include<stdio.h>

int HandleClientHTTPRequest(char* szRequest, int nRequestSize, SOCKET skClientSocket);
int Handle_ExtMethods(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Get(Dictionary *dictHttpRequest, SOCKET scClientSocket);
int Handle_Head(Dictionary *dictHttpRequest, SOCKET scClientSocket);
int Handle_Options(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Delete(Dictionary *dictHttpRequest, SOCKET scClientSocket);
int Handle_Trace(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Post(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Put(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);
int Handle_Connect(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket);

typedef struct
{
	int MethodOfRequest;
	char *strURIOfRequest;
	int VersionOfHttp;
}HttpInitialRequestLine;

extern char *strMappedLocalPath;

#endif
