#ifndef HANDLEHTTPRESPONSE_H
#define HANDLEHTTPRESPONSE_H

#include"SupportingDataStructures.h"
#include<stdlib.h>
#include<Windows.h>
#include<stdio.h>
#include "Shlwapi.h"
#include "SupportingFunctions.h"

//HTTP Response codes

#define HTTP_RESPONSE_CODE_CONTINUE 100
#define HTTP_RESPONSE_CODE_SWITCH_PROTOCOL 101

//Success codes
#define HTTP_RESPONSE_CODE_OK 200
#define HTTP_RESPONSE_CODE_CREATED 201
#define HTTP_RESPONSE_CODE_ACCEPTED 202
#define HTTP_RESPONSE_CODE_PARTIAL_INFORMATION 203
#define HTTP_RESPONSE_CODE_NO_RESPONSE 204
#define HTTP_RESPONSE_CODE_RESET_CONTENT 205
#define HTTP_RESPONSE_CODE_PARTIAL_CONTENT 206

//HTTP Response Redirection codes
#define HTTP_RESPONSE_CODE_MULTIPLE_CHOICES 300
#define HTTP_RESPONSE_CODE_MOVED 301
#define HTTP_RESPONSE_CODE_FOUND 302
#define HTTP_RESPONSE_CODE_METHOD 303
#define HTTP_RESPONSE_CODE_NOT_MODIFIED 304
#define HTTP_RESPONSE_USE_PROXY 305
#define HTTP_RESPONSE_UNUSED 306
#define HTTP_RESPONSE_TEMP_REDIRECT 307

//HTTP Response Error codes
#define HTTP_RESPONSE_CODE_BAD_REQUEST 400
#define HTTP_RESPONSE_CODE_UNAUTHORIZED 401
#define HTTP_RESPONSE_CODE_PAYMENTREQUIRED 402
#define HTTP_RESPONSE_CODE_FORBIDDEN 403
#define HTTP_RESPONSE_CODE_NOT_FOUND 404
#define HTTP_RESPONSE_CODE_METHOD_NOT_ALLOWED 405
#define HTTP_RESPONSE_CODE_NOT_ACCEPTABLE 406
#define HTTP_RESPONSE_CODE_PROXY_AUTH_REQ 407
#define HTTP_RESPONSE_CODE_REQUEST_TIMEOUT 408
#define HTTP_RESPONSE_CODE_CONFLICT 409
#define HTTP_RESPONSE_CODE_GONE 410
#define HTTP_RESPONSE_CODE_LEN_REQUIRED 411
#define HTTP_RESPONSE_CODE_PRECOND_REQ 412
#define HTTP_RESPONSE_CODE_REQ_ENTITY_TOO_LARGE 413
#define HTTP_RESPONSE_CODE_REQ_URI_TOO_LONG 414
#define HTTP_RESPONSE_CODE_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_RESPONSE_CODE_REQ_RANGE_NOT_SATISFIABLE 416
#define HTTP_RESPONSE_CODE_EXPECTATION_FAILED 417

//SERVER side errors
#define HTTP_RESPONSE_CODE_INTERNAL_ERROR 500
#define HTTP_RESPONSE_CODE_NOT_IMPLEMENTED 501
#define HTTP_RESPONSE_CODE_SERV_BAD_GATEWAY 502
#define HTTP_RESPONSE_CODE_SERV_UNAVAILABLE 503
#define HTTP_RESPONSE_CODE_SERV_TIMEOUT 504
#define HTTP_RESPONSE_CODE_HTTP_VER_NOT_SUP 505



char *str200Status = "OK";
char *str201Status = "Resource Created";
char *str202Status = "Accepted for processing";
char *str203Status = "Non authoritative information";
char *str204Status = "Full fullied request. No content";

char *str500Status = "Method not implemented";
char *str501Status = "Method not implemented";
char *str502Status = "Method not implemented";
char *str503Status = "Method not implemented";

char *str400Status = "Method not implemented";
char *str401Status = "Method not implemented";
char *str402Status = "Method not implemented";
char *str403Status = "Method not implemented";
char *str404Status = "Method not implemented";

char *str301Status = "Method not implemented";
char *str302Status = "Method not implemented";
char *str303Status = "Method not implemented";
char *str304Status = "Method not implemented";

extern char *strMappedLocalPath;

int HandleGetFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char *strFileToSend);
int HandleFileNotFound(Dictionary *dictHttpRequest, SOCKET scClientSocket);
int HandleHeadFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket);
int HandleHeadFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket);
int HandleDeleteFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char* szFilePath);

#endif
