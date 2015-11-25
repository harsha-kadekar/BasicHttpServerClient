#ifndef HANDLEHTTPREQUEST_H
#define HANDLEHTTPREQUEST_H

#include"ErrorCodes.h"
#include"HttpRequestParsing.h"
#include"SupportingDataStructures.h"
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


//HTTP Response Success codes
#define HTTP_RESPONSE_CODE_OK 200
#define HTTP_RESPONSE_CODE_CREATED 201
#define HTTP_RESPONSE_CODE_ACCEPTED 202
#define HTTP_RESPONSE_CODE_PARTIAL_INFORMATION 203
#define HTTP_RESPONSE_CODE_NO_RESPONSE 204

//HTTP Response Error codes
#define HTTP_RESPONSE_CODE_BAD_REQUEST 400
#define HTTP_RESPONSE_CODE_UNAUTHORIZED 401
#define HTTP_RESPONSE_CODE_PAYMENTREQUIRED 402
#define HTTP_RESPONSE_CODE_FORBIDDEN 403
#define HTTP_RESPONSE_CODE_NOT_FOUND 404
#define HTTP_RESPONSE_CODE_INTERNAL_ERROR 500
#define HTTP_RESPONSE_CODE_NOT_IMPLEMENTED 501
#define HTTP_RESPONSE_CODE_SERV_TEMP_OVERLOADED 502
#define HTTP_RESPONSE_CODE_GATEWAY_TIMEOUT 503

//HTTP Response Redirection codes
#define HTTP_RESPONSE_CODE_MOVED 301
#define HTTP_RESPONSE_CODE_FOUND 302
#define HTTP_RESPONSE_CODE_METHOD 303
#define HTTP_RESPONSE_CODE_NOT_MODIFIED 304

typedef struct
{
	int MethodOfRequest;
	char *strURIOfRequest;
	int VersionOfHttp;
}HttpInitialRequestLine;



#endif
