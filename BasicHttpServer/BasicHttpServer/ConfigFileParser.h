#ifndef CONFIGFILEPARSER_H
#define CONFIGFILEPARSER_H

#include<stdio.h>
#include<Windows.h>
#include"ErrorCodes.h"
#include"SupportingFunctions.h"

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

int ConfigFileParserInitialization();
int ReadConfigurationFile();
int HandleServerConfiguration(char* szFileBuffer, int nSize, int *nCurrentIndex);
int HandleHostedWebSites(char* szFileBuffer, int nSize, int *nCurrentIndex);
int GetWebSiteDetails(char* szFileBuffer, int nSize, int *nCurrentIndex, WebSiteDetails *webSite);
int UninitializeConfigurationParameters();

#endif
