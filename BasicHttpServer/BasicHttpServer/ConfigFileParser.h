#ifndef CONFIGFILEPARSER_H
#define CONFIGFILEPARSER_H

#include<stdio.h>
#include<Windows.h>
#include"ErrorCodes.h"

int nMaxClientRequestSize = 8192;
int nThreadPoolSize = 512;

typedef struct
{
	char* strWebSiteName;
	char* strIPAddress;
	char* strLocalPath;
	char* strAliasNames;
}WebSiteDetails;

typedef struct structWebSites
{
	WebSiteDetails* webDetails;
	struct structWebSites	*pNext;
}WebSites;

WebSites* pHeadOfWebSiteList;

char strConfigFilePath[260] = "BasicHttpServerConfig.config";

int HandleServerConfiguration(char* szFileBuffer, int nSize, int nCurrentIndex);
int HandleHostedWebSites(char* szFileBuffer, int nSize, int *nCurrentIndex);
int ConvertStringToNumber(char* strNum, int* pResNum);

#endif
