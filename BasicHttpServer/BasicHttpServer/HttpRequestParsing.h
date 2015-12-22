#pragma once
#include<stdio.h>
#include"ErrorCodes.h"
#include"SupportingDataStructures.h"
#include"SupportingFunctions.h"
#include<Windows.h>
#include<string.h>
#include "Shlwapi.h"

#define METHOD_OPTIONS 0
#define METHOD_GET 1
#define METHOD_HEAD 2
#define METHOD_POST 3
#define METHOD_PUT 4
#define METHOD_DELETE 5
#define METHOD_TRACE 6
#define METHOD_CONNECT 7
#define METHOD_EXTEND_METHODS 8

int GetTypeOfMethod(char* szRecievedMethod);
int GetHTTPVersionOfRequest(char *szRequestHTTPVersion);
int GenerateHttpRequestDictionary(char *szHttpRequest, int nRequestSize, Dictionary* dictHttpReq);
char* GetFilePathFromURI(char *szURI, char *szLocalPath);
int FindFileInLocalPath(char *szURI, char *szLocalPath);

