/*
This file contains functions which will read the configuraiton file of the BasicHttpServer.
Developer - Harsha
*/

#include"ConfigFileParser.h"

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

	return nReturnValue;

}

/*
Name: ReadConfigurationFile
Description: This function is responsible for reading the configuraiton file and setting necessary parameters.
Parameter: - 
ReturnValue: 0 for success else error codes.
*/
int ReadConigurationFile()
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

				if (strcmp(szKey, "HostedWebSites"))
				{
					nReturnValue = HandleHostedWebSites(szFileBuffer, nNumberOfCharRead, &i);
					if (nReturnValue != 0)
						return nReturnValue;
				}

				if (strcmp(szKey, "ServerConfiguration"))
				{
					nReturnValue = HandleServerConfiguration(szFileBuffer, nNumberOfCharRead, &i);
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

	for (int i = *(nCurrentIndex); i < nSize; i++)
	{
		if (szFileBuffer[i] != '<')
			continue;
		else
		{
			if (szFileBuffer[i + 1] != '/')
			{
				memset(szKey, '\0', 1024);
				j = 0;
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}
				if (strcmp(szKey, "ThreadPoolSize"))
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
				else if (strcmp(szKey, "MaximumSizeOfRequest"))
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
				else
				{
					//Not recognizable key!!!! error
					nReturnValue = ERR_INVALID_KEY_IN_CONFIG;
					return nReturnValue;
				}
			}
			else
			{
				memset(szKey, '\0', 1024);
				j = 0;
				while (szFileBuffer[i] != '>')
				{
					szKey[j++] = szFileBuffer[i++];
				}

				if (strcmp(szKey, "/ServerConfiguration"))
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
Name: ConvertStringToNumber
Description: As the name suggests this function is responsible for converting the read string into number.
Parameter: strNum = Number in the format of strings
		   pResNum = after conversion, number will be placed here.
ReturnValue: 0 for success, else -ve error code.
*/
int ConvertStringToNumber(char* strNum, int* pResNum)
{
	int nReturnValue = 0;
	int i = 0;
	int nNumber = 0;
	int nTemp = 0;

	while (strNum[i] != '\0' && i < 1024)
	{
		nTemp = strNum[i] - '0';
		if (nTemp > 9)
		{
			//INVALID NUMBER CHARACTER
			nReturnValue = ERR_INVALID_VALUE_IN_CONFIG;
			return nReturnValue;
		}

		nNumber = nNumber * 10 + nTemp;

	}

	(*pResNum) = nNumber;

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

	return nReturnValue;
}