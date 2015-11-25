#pragma once
#include<stdio.h>
#include"ErrorCodes.h"
#include"SupportingDataStructures.h"
#include<Windows.h>
#include<string.h>

#define METHOD_OPTIONS 0
#define METHOD_GET 1
#define METHOD_HEAD 2
#define METHOD_POST 3
#define METHOD_PUT 4
#define METHOD_DELETE 5
#define METHOD_TRACE 6
#define METHOD_CONNECT 7
#define METHOD_EXTEND_METHODS 8

int GetTypeOfMethod(char* szRequest, int nRequestCount);
int GetHTTPVersionOfRequest(char *szRequest, int nCount);
char* GetURIOfRequest(char* szRequest, int nCount, int nMethod);
int GenerateHttpRequestDictionary(char *szHttpRequest, int nRequestSize, Dictionary* dictHttpReq);
