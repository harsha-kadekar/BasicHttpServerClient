/*
This file contains functions which will read the configuraiton file of the BasicHttpServer.
Developer - Harsha
*/

#include"ConfigFileParser.h"

char strConfigFilePath[260] = "BasicHttpServerConfig.config";
int nMaxClientRequestSize = 8192;
int nThreadPoolSize = 512;
int nConfiguredPortNumbered = 8080;
int nLogLevel = 1;
char *strMappedLocalPath = 0;
char *strLogFilePath = 0;
int bOnlyFileLogging = 1;
int NoOfDaysBackUpLogFile = 30;


/*
Name: ConfigFileParserInitialization
Description: This function will do the initialization of the parameters. Also does necessary tasks to start reading configuration file.
Parameter: - 
ReturnValue: 0 for success, else -ve value indicating error.
*/
int ConfigFileParserInitialization()
{
	int nReturnValue = 0;

	pHeadOfWebSiteList = 0;
	nMaxClientRequestSize = 8192;
	nThreadPoolSize = 512;
	nConfiguredPortNumbered = 8080;
	nLogLevel = 1;
	strMappedLocalPath = 0;
	strLogFilePath = 0;
	bOnlyFileLogging = 1;
	NoOfDaysBackUpLogFile = 30;

	return nReturnValue;

}

/*
Name: ReadConfigurationFile
Description: This function is responsible for reading the configuraiton file and setting necessary parameters.
Parameter: - 
ReturnValue: 0 for success else error codes.
*/
int ReadConfigurationFile()
{
	int nReturnValue = 0;
	FILE *fp = NULL;
	char szFileBuffer[8192] = { '\0' };
	char szLine[1024] = { '\0' };
	char szKey[1024] = { '\0' };
	

	size_t nNumberOfCharRead = 0;
	int nCompletedReading = 0;
	int i = 0, j = 0;

	nReturnValue = fopen_s(&fp, strConfigFilePath, "r");
	if (nReturnValue != 0)
		return nReturnValue;
	//fp = fopen(strConfigFilePath, "r");
	if (fp == 0)
	{
		nReturnValue = ERR_OPENNING_CONFIGFILE;
		return nReturnValue;
	}

	nNumberOfCharRead = fread_s(szFileBuffer, 8192, sizeof(char), 8192, fp);
	if (nNumberOfCharRead == 0)
	{
		nReturnValue = ERR_READING_CONFIGFILE_ERROR;
		fclose(fp);
		fp = 0;
		return nReturnValue;
	}

	if (nNumberOfCharRead < 8192)
	{
		nCompletedReading = 1;
	}

	fclose(fp);
	fp = 0;

	for (i = 0; i < nNumberOfCharRead; i++)
	{
		if (szFileBuffer[i] == '<')
		{
			i++;
			if (szFileBuffer[i] != '/')
			{
				//Start of a key
				j = 0;
				memset(szKey, '\0', 1024);
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "HostedWebSites") == 0)
				{
					nReturnValue = HandleHostedWebSites(szFileBuffer, nNumberOfCharRead, &i);
					if (nReturnValue != 0)
						return nReturnValue;
				}

				else if (strcmp(szKey, "ServerConfiguration") == 0)
				{
					nReturnValue = HandleServerConfiguration(szFileBuffer, nNumberOfCharRead, &i);
					if (nReturnValue != 0)
					{
						return nReturnValue;
					
					}
				}

				else if (strcmp(szKey, "LoggingConfig") == 0)
				{
					nReturnValue = HandleLoggingConfiguration(szFileBuffer, nNumberOfCharRead, &i);
					if (nReturnValue != 0)
					{
						return nReturnValue;

					}
				}

			}
			else
			{
				j = 0;
				memset(szKey, '\0', 1024);
				while (szFileBuffer[i] != '>')
					szKey[j++] = szFileBuffer[i++];

				if (strcmp(szKey, "/ServerConfig"))
					break;
			}
		}

	}
	

	return nReturnValue;
}

int HandleLoggingConfiguration(char *szFileBuffer, int nSize, int *nCurrentIndex)
{
	int nReturnValue = 0;
	int i = 0;
	int j = 0;
	int nTempNum = 0;
	char szKey[1024] = { '\0' };

	/*
	<LogFilePath>"C:\D_Drive\Coding\BasicHttpServerClient\BasicHttpServer\Log"< / LogFilePath>
	<LogLevel>DEBUG< / LogLevel>
	<OnlyFile>FALSE< / OnlyFile>
	<BackupDays>30< / BackupDays>
	*/

	for (i = *(nCurrentIndex); i < nSize; i++)
	{
		if (szFileBuffer[i] != '<')
			continue;
		else
		{
			if (szFileBuffer[i + 1] != '/')
			{
				i++;
				memset(szKey, '\0', 1024);
				j = 0;
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "LogFilePath") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];

					strLogFilePath = (char*)malloc(sizeof(char)*strnlen_s(szKey, 1024) + 1);
					memset(strLogFilePath, '\0', strnlen_s(szKey, 1024) + 1);

					stringcopy(strLogFilePath, strnlen_s(szKey, 1024), szKey);

				}
				else if (strcmp(szKey, "LogLevel") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];

					if (strcmp(szKey, "ALL") == 0)
						nLogLevel = 1;
					else if (strcmp(szKey, "DEBUG") == 0)
						nLogLevel = 2;
					else if (strcmp(szKey, "INFO") == 0)
						nLogLevel = 3;
					else if (strcmp(szKey, "WARN") == 0)
						nLogLevel = 4;
					else
						nLogLevel = 5;	//ERROR

				}
				else if (strcmp(szKey, "OnlyFile") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					nTempNum = 0;
					nReturnValue = ConvertStringToNumber(szKey, &nTempNum);
					if (nReturnValue != 0)
					{
						return ERR_INVALID_VALUE_IN_CONFIG;
					}
					bOnlyFileLogging = nTempNum;

				}
				else if (strcmp(szKey, "BackupDays") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];

					nTempNum = 0;
					nReturnValue = ConvertStringToNumber(szKey, &nTempNum);
					if (nReturnValue != 0)
					{
						return ERR_INVALID_VALUE_IN_CONFIG;
					}
					NoOfDaysBackUpLogFile = nTempNum;

				}
				else
				{
					//Not recognizable key!!!! error
					nReturnValue = ERR_INVALID_KEY_IN_CONFIG;
					return nReturnValue;
				}



			}
			else
			{
				i++;
				memset(szKey, '\0', 1024);
				j = 0;
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "/LoggingConfig") == 0)
				{
					break;
				}

			}

		}

	}

	return nReturnValue;
}

/*
Name: HandleServerConfiguration
Description: This function will read the ServerConfiguraiton part of config file and set the parameters.
Parameter: szFileBuffer - Full content of the config file
		   nSize - Size of the buffer
		   *nCurrentIndex - this is the pointer pointing to the current index of the buffer.
nReturnValue: 0 for success else -ve error codes.
*/
int HandleServerConfiguration(char* szFileBuffer, int nSize, int *nCurrentIndex)
{
	int nReturnValue = 0;
	int i = 0;
	int j = 0;
	int nTempNum = 0;
	char szKey[1024] = { '\0' };

	/*
	<ThreadPoolSize>512</ThreadPoolSize>
	<MaximumSizeOfRequest>8192</MaximumSizeOfRequest>
	*/

	for (i = *(nCurrentIndex); i < nSize; i++)
	{
		if (szFileBuffer[i] != '<')
			continue;
		else
		{
			if (szFileBuffer[i + 1] != '/')
			{
				i++;
				memset(szKey, '\0', 1024);
				j = 0;
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}
				if (strcmp(szKey, "ThreadPoolSize") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					nTempNum = 0;
					nReturnValue = ConvertStringToNumber(szKey, &nTempNum);
					if (nReturnValue != 0)
					{
						return ERR_INVALID_VALUE_IN_CONFIG;
					}
					nThreadPoolSize = nTempNum;


				}
				else if (strcmp(szKey, "MaximumSizeOfRequest") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					nTempNum = 0;
					nReturnValue = ConvertStringToNumber(szKey, &nTempNum);
					if (nReturnValue != 0)
					{
						return ERR_INVALID_VALUE_IN_CONFIG;
					}
					nMaxClientRequestSize = nTempNum;

				}
				else if (strcmp(szKey, "LocalMappedPath") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					strMappedLocalPath = (char*)malloc(sizeof(char)*strnlen_s(szKey, 1024) + 1);
					memset(strMappedLocalPath, '\0', strnlen_s(szKey, 1024) + 1);
					j = 0;
					while (szKey[j] != '\0' && j < 1024)
					{
						strMappedLocalPath[j] = szKey[j];
						j++;
					}

				}
				/*else if (strcmp(szKey, "LogLevel") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];

					if (strcmp(szKey, "ALL") == 0)
						nLogLevel = 1;
					else if (strcmp(szKey, "DEBUG") == 0)
						nLogLevel = 2;
					else if (strcmp(szKey, "INFO") == 0)
						nLogLevel = 3;
					else if (strcmp(szKey, "WARN") == 0)
						nLogLevel = 4;
					else
						nLogLevel = 5;	//ERROR

				}*/
				else if (strcmp(szKey, "PortToWatch") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];

					nTempNum = 0;
					nReturnValue = ConvertStringToNumber(szKey, &nTempNum);
					if (nReturnValue != 0)
					{
						return ERR_INVALID_VALUE_IN_CONFIG;
					}
					nConfiguredPortNumbered = nTempNum;

				}
				else
				{
					//Not recognizable key!!!! error
					nReturnValue = ERR_INVALID_KEY_IN_CONFIG;
					return nReturnValue;
				}
			}
			else
			{
				i++;
				memset(szKey, '\0', 1024);
				j = 0;
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "/ServerConfiguration") == 0)
				{
					break;
				}

			}
		}
	}

	(*nCurrentIndex) = i;
	return nReturnValue;
}



/*
Name: HandleHostedWebSites
Description: This will parse the WebSites part of the config file. Try to store the websites hosted in the server information.
Parameter: szFileBuffer - Config file in memory
		   nSize - Size of the file buffer
		   *nCurrentIndex - Index from which websites section start
nReturnValue: 0 for success else -ve value
*/
int HandleHostedWebSites(char* szFileBuffer, int nSize, int *nCurrentIndex)
{
	int nReturnValue = 0;
	int i = 0;
	int j = 0;
	int nTempNum = 0;
	char szKey[1024] = { '\0' };

	/*<WebSite>
		<LocalMappedPath>"C:\D_Drive\Coding\BasicHttpServerClient\BasicHttpServer\Webpages"< / LocalMappedPath>
		<WebSiteName>www.justfun.com< / WebSiteName>
		<AliasWebSiteName>< / AliasWebSiteName>
		<IPAddress>10.8.20.9< / IPAddress>
	< / WebSite>*/

	for (i = (*nCurrentIndex); i < nSize; i++)
	{
		if (szFileBuffer[i] != '<')
			continue;
		else
		{
			i++;
			if (szFileBuffer[i] != '/')
			{
				j = 0;
				memset(szKey, '\0', 1024);
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "WebSite") == 0)
				{
					WebSiteDetails *webSite = (WebSiteDetails*)malloc(sizeof(WebSiteDetails) * 1);

					nReturnValue = GetWebSiteDetails(szFileBuffer, nSize, &i, webSite);
					if (nReturnValue != 0)
					{
						free(webSite);
						webSite = 0;
						return nReturnValue;
					}

					WebSites *webSiteNode = (WebSites*)malloc(sizeof(WebSites) * 1);
					(*webSiteNode).webDetails = webSite;
					(*webSiteNode).pNext = 0;

					if (pHeadOfWebSiteList != 0)
					{
						(*webSiteNode).pNext = pHeadOfWebSiteList;
						pHeadOfWebSiteList = webSiteNode;
					}
					else
					{
						pHeadOfWebSiteList = webSiteNode;
					}
				}


			}
			else
			{
				i++;
				j = 0;
				memset(szKey, '\0', 1024);
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "HostedWebSites") == 0)
					break;
			}
		}
		
	}

	(*nCurrentIndex) = i;

	return nReturnValue;
}

/*
Name: GetWebSiteDetails
Description: This will read specific website details from the configuration file.
parameters: szFileBuffer - configuraition file in memory
			nSize - Size of the file
			nCurrentIndex - Point where the website details is going to start
			webSite - structure where read information needs to be stored.
returnValue: 0 for success else -ve value.
*/
int GetWebSiteDetails(char* szFileBuffer, int nSize, int *nCurrentIndex, WebSiteDetails *webSite)
{
	int nReturnValue = 0;
	int i = 0;
	int j = 0;
	char szKey[1024] = { '\0' };

	/*
	<LocalMappedPath>"C:\D_Drive\Coding\BasicHttpServerClient\BasicHttpServer\Webpages"< / LocalMappedPath>
	<WebSiteName>www.justfun.com< / WebSiteName>
	<AliasWebSiteName>< / AliasWebSiteName>
	<IPAddress>10.8.20.9< / IPAddress>
	*/

	for (i = (*nCurrentIndex); i < nSize; i++)
	{
		if (szFileBuffer[i] != '<')
			continue;
		else
		{
			if (szFileBuffer[i + 1] != '/')
			{
				i++;
				j = 0;
				memset(szKey, '\0', 1024);
				while (szFileBuffer[i] != '>')
					szKey[j++] = szFileBuffer[i++];

				if (strcmp(szKey, "LocalMappedPath") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					(*webSite).strLocalPath = (char*)malloc(sizeof(char)*strnlen_s(szKey, 1024)+1);
					memset((*webSite).strLocalPath, '\0', strnlen_s(szKey, 1024) + 1);
					j = 0;
					while (j < strnlen_s(szKey, 1024))
					{
						(*webSite).strLocalPath[j] = szKey[j];
						j++;
					}

					//strcpy_s((*webSite).strLocalPath, strnlen_s(szKey, 1024), szKey);

					
				}
				else if (strcmp(szKey, "WebSiteName") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					(*webSite).strWebSiteName = (char*)malloc(sizeof(char)*strnlen_s(szKey, 1024)+1);
					memset((*webSite).strWebSiteName, '\0', strnlen_s(szKey, 1024) + 1);
					j = 0;
					while (j < strnlen_s(szKey, 1024))
					{
						(*webSite).strWebSiteName[j] = szKey[j];
						j++;
					}
				}
				else if (strcmp(szKey, "AliasWebSiteName") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					(*webSite).strAliasNames = (char*)malloc(sizeof(char)*strnlen_s(szKey, 1024)+1);
					memset((*webSite).strAliasNames, '\0', strnlen_s(szKey, 1024) + 1);
					j = 0;
					while (j < strnlen_s(szKey, 1024))
					{
						(*webSite).strAliasNames[j] = szKey[j];
						j++;
					}
				}
				else if (strcmp(szKey, "IPAddress") == 0)
				{
					i++;
					j = 0;
					memset(szKey, '\0', 1024);
					while (szFileBuffer[i] != '<')
						szKey[j++] = szFileBuffer[i++];
					(*webSite).strIPAddress = (char*)malloc(sizeof(char)*strnlen_s(szKey, 1024)+1);
					memset((*webSite).strIPAddress, '\0', strnlen_s(szKey, 1024) + 1);
					j = 0;
					while (j < strnlen_s(szKey, 1024))
					{
						(*webSite).strIPAddress[j] = szKey[j];
						j++;
					}
				}

			}
			else
			{
				i++;
				j = 0;
				memset(szKey, '\0', 1024);
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "/WebSite") == 0)
					break;
			}
		}
	}

	(*nCurrentIndex) = i;

	return nReturnValue;
}

/*
Name: UninitializeConfigurationParameters
Description: This function will uninitialize the parameters which are set during configuration file reading. It involves deallocating 
			 memory as well.
Parameters: -
ReturnValue: 0 for success else -ve value indicating error codes.
*/
int UninitializeConfigurationParameters()
{
	int nReturnValue = 0;
	WebSites *pCurrent = 0, *pTemp = 0;

	if (pHeadOfWebSiteList != 0)
	{
		pCurrent = pHeadOfWebSiteList;
		while (pCurrent != 0)
		{
			free((*(*pCurrent).webDetails).strAliasNames);
			(*(*pCurrent).webDetails).strAliasNames = 0;
			free((*(*pCurrent).webDetails).strIPAddress);
			(*(*pCurrent).webDetails).strIPAddress = 0;
			free((*(*pCurrent).webDetails).strLocalPath);
			(*(*pCurrent).webDetails).strLocalPath = 0;
			free((*(*pCurrent).webDetails).strWebSiteName);
			(*(*pCurrent).webDetails).strWebSiteName = 0;

			free((*pCurrent).webDetails);
			(*pCurrent).webDetails = 0;

			pTemp = pCurrent;
			pCurrent = (*pCurrent).pNext;

			free(pTemp);
			pTemp = 0;
		}

		pHeadOfWebSiteList = 0;
	}

	if (strMappedLocalPath != 0)
	{
		free(strMappedLocalPath);
		strMappedLocalPath = 0;
	}

	return nReturnValue;
}