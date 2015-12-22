#include"HttpRequestParsing.h"




/*
Name: GetTypeOfMethod
Description: This method is responsible for finding the method of HTTP request.
Parameter: Starting point of the RecieveMsg[THis is may not be complete request]
nCount: Length of the request
return: Type of method - each encoded by a numeric value OPTIONS:0, GET:1, HEAD:2, POST:3, PUT:4, DELETE:5, TRACE:6, CONNECT:7, EXTENDED:8
*/
int GetTypeOfMethod(char* szRecievedMethod)
{
	int nReturnValue = 0;
	char* szOption = "OPTIONS";
	char* szGet = "GET";
	char* szHead = "HEAD";
	char* szPost = "POST";
	char* szPut = "PUT";
	char* szDelete = "DELETE";
	char* szTrace = "TRACE";
	char* szConnect = "CONNECT";

	if (0 == szRecievedMethod)
	{
		printf_s("BHS::ERROR: request string is empty\n");
		return ERR_REQUESTSTRINGNULL;
	}

	/*
	Method = "OPTIONS" 0; 
		| "GET" 1; 
		| "HEAD" 2; 
		| "POST" 3; 
		| "PUT" 4; 
		| "DELETE" 5; 
		| "TRACE" 6; 
		| "CONNECT" 7; 
		| extension - method 8
		extension - method = token
	*/

	if (strcmp(szRecievedMethod, szConnect) == 0)
		nReturnValue = METHOD_CONNECT;
	else if (strcmp(szRecievedMethod, szOption) == 0)
		nReturnValue = METHOD_OPTIONS;
	else if (strcmp(szRecievedMethod, szDelete) == 0)
		nReturnValue = METHOD_DELETE;
	else if (strcmp(szRecievedMethod, szTrace) == 0)
		nReturnValue = METHOD_TRACE;
	else if (strcmp(szRecievedMethod, szPost) == 0)
		nReturnValue = METHOD_POST;
	else if (strcmp(szRecievedMethod, szHead) == 0)
		nReturnValue = METHOD_HEAD;
	else if (strcmp(szRecievedMethod, szPut) == 0)
		nReturnValue = METHOD_PUT;
	else if (strcmp(szRecievedMethod, szGet) == 0)
		nReturnValue = METHOD_GET;
	else
		nReturnValue = ERR_CANNOTIDENTIFYMETHOD;

	return nReturnValue;
}



/*
Name: GetHTTPVersionOfRequest
Description: This function will return the version of the http request - is it HTTP 1.1 or HTTP 1.0
Parameters: szRequest - Actual http request
			nCount - Length of the http request
ReturnValue: 0 for HTTP 1.1, 1 for HTTP 1.0 else -ve error codes in case of error
*/
int GetHTTPVersionOfRequest(char *szHttpRequestVersion)
{
	int nReturnValue = ERR_NOT_FOUND_HTTP_VER;
	char *szHttp1_1 = "HTTP/1.1";
	char *szHttp1_0 = "HTTP/1.0";

	//GET /pub/WWW/TheProject.html HTTP/1.1
	//GET /pub/WWW/TheProject.html HTTP/1.0

	if (szHttpRequestVersion == 0 )
		return ERR_INVALID_REQUEST;

	if (strcmp(szHttpRequestVersion, szHttp1_1) == 0)
		nReturnValue = 0;
	else if (strcmp(szHttpRequestVersion, szHttp1_0) == 0)
		nReturnValue = 1;
	else
		nReturnValue = ERR_NOT_FOUND_HTTP_VER;

	return nReturnValue;
}

/*
Name: GenerateHttpRequestDictionary
Description: This function given a string of complete HTTP Request converts them to name value pairs and stores in the dictionary.
Parameters: szHttpRequest: This is the complete HTTP request string.
			nRequestSize: Size fo the HTTP request string
			dictHttpReq: Dictionary where the name value pair will be stored.
ReturnVaue: 0 for success else error.
*/
int GenerateHttpRequestDictionary(char *szHttpRequest, int nRequestSize, Dictionary* dictHttpReq)
{
	int nReturnValue = 0;
	int nLineNo = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int nStage = 0;
	char *chTempBuffer = 0;
	char *chTempBuffer2 = 0;
	char szEndOfLine[3] = { '\0' };
	char *szName = 0;
	char *szValue = 0;

	szEndOfLine[0] = 13;
	szEndOfLine[1] = 10;

	for (i = 0; i < nRequestSize; i++)
	{
		if (szHttpRequest[i] == '\n')
		{
			chTempBuffer = (char*)malloc(sizeof(char)*((i - j) + 1));
			if (chTempBuffer == 0)
			{
				return ERR_INVALID_MEMORY_OPERATION;
			}
			chTempBuffer2 = (char*)malloc(sizeof(char)*((i - j) + 1));
			if (chTempBuffer2 == 0)
			{
				return ERR_INVALID_MEMORY_OPERATION;
			}

			memset(chTempBuffer, '\0', (i - j) + 1);
			memset(chTempBuffer2, '\0', (i - j) + 1);

			for (k = j, l = 0; k < i; k++, l++)
			{
				chTempBuffer[l] = szHttpRequest[k];
			}

			if (nLineNo == 0)
			{
				//Process 1st initial line
				k = 0;
				l = 0;
				while (k < (i - j) + 1)
				{
					if (chTempBuffer[k] != ' ' && chTempBuffer[k] != '\r')
					{
						chTempBuffer2[l] = chTempBuffer[k];
					}
					else
					{
						if (nStage == 0)
						{
							/*
							Method = "OPTIONS" 0;
							| "GET" 1;
							| "HEAD" 2;
							| "POST" 3;
							| "PUT" 4;
							| "DELETE" 5;
							| "TRACE" 6;
							| "CONNECT" 7;
							| extension - method 8
							extension - method = token
							*/

							szName = (char*)malloc(sizeof(char) * 7);
							if (szName == 0)
							{
								//Error handle it
							}
							memset(szName, '\0', 7);
							stringcopy(szName, 6, "METHOD");
							//strcpy_s(szName, 6, "METHOD");

							if (strcmp(chTempBuffer2, "GET") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 4);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 4);
								stringcopy(szValue, 4, "GET");
								//strcpy_s(szValue, 4, "GET");
							}
							else if (strcmp(chTempBuffer2, "OPTIONS")== 0)
							{
								szValue = (char*)malloc(sizeof(char) * 8);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 8);
								stringcopy(szValue, 8, "OPTIONS");
								//strcpy_s(szValue, 8, "OPTIONS");

							}
							else if (strcmp(chTempBuffer2, "HEAD") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 5);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 5);
								stringcopy(szValue, 5, "HEAD");
								//strcpy_s(szValue, 5, "HEAD");
							}
							else if (strcmp(chTempBuffer2, "POST") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 5);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 5);
								stringcopy(szValue, 5, "POST");
								//strcpy_s(szValue, 5, "POST");

							}
							else if (strcmp(chTempBuffer2, "PUT") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 4);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 4);
								stringcopy(szValue, 4, "PUT");
								//strcpy_s(szValue, 4, "PUT");
							}
							else if (strcmp(chTempBuffer2, "DELETE") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 7);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 7);
								stringcopy(szValue, 7, "DELETE");
								//strcpy_s(szValue, 7, "DELETE");
							}
							else if (strcmp(chTempBuffer2, "TRACE") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 6);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 6);
								stringcopy(szValue, 6, "TRACE");
								//strcpy_s(szValue, 6, "TRACE");
							}
							else if (strcmp(chTempBuffer2, "CONNECT") == 0)
							{
								szValue = (char*)malloc(sizeof(char) * 8);
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', 8);
								stringcopy(szValue, 4, "CONNECT");
								//strcpy_s(szValue, 4, "CONNECT");

							}
							else
							{

							}
							AddNameValueToDictionary(dictHttpReq, szName, szValue);
							nStage++;
						}
						else if (nStage == 1)
						{
							szName = (char*)malloc(sizeof(char) * 4);
							if (szName == 0)
							{
								//Error handle it
							}
							memset(szName, '\0', 4);
							stringcopy(szName, 4, "URI");
							//strcpy_s(szName, 4, "URI");

							szValue = (char*)malloc(sizeof(char) * l+1);
							if (szValue == 0)
							{
								//Error handle it
							}
							memset(szValue, '\0', l + 1);
							stringcopy(szValue, l+1, chTempBuffer2);
							//strcpy_s(szValue, l + 1, chTempBuffer2);

							AddNameValueToDictionary(dictHttpReq, szName, szValue);
							nStage++;

						}
						else
						{
							szName = (char*)malloc(sizeof(char) * 11);
							if (szName == 0)
							{
								//Error handle it
							}
							memset(szName, '\0', 11);
							stringcopy(szName, 11, "HTTPVERSION");
							//strcpy_s(szName, 11, "HTTPVERSION");

							szValue = (char*)malloc(sizeof(char)*l + 1);
							if (szValue == 0)
							{
								//Error handle it
							}
							memset(szValue, '\0', l + 1);
							stringcopy(szValue, l+1, chTempBuffer2);
							//strcpy_s(szValue, l + 1, chTempBuffer2);

							nReturnValue = AddNameValueToDictionary(dictHttpReq, szName, szValue);
							if (nReturnValue != 0)
							{
								//Error handle it
							}
							nStage++;
						}
						memset(chTempBuffer2, '\0', (i - j) + 1);
						l = -1;
					}

					l++;
					k++;
				}
			}
			else
			{
				//Process header lines
				k = 0;
				l = 0;
				if(chTempBuffer[0] != '\r')
				{
					while (k < (i - j) + 1)
					{
						if (chTempBuffer[k] != ':' && chTempBuffer[k] != '\r')
						{
							chTempBuffer2[l] = chTempBuffer[k];
						}
						else
						{
							//Process it
							if (chTempBuffer[k] == ':')
							{
								//Got the name
								szName = (char*)malloc(sizeof(char)*(l + 1));
								if (szName == 0)
								{
									//Error handle it
								}
								memset(szName, '\0', l + 1);
								stringcopy(szName, l + 1, chTempBuffer2);
								//strcpy_s(szName, l + 1, chTempBuffer2);

							}
							else
							{
								//Got the value
								szValue = (char*)malloc(sizeof(char)*(l + 1));
								if (szValue == 0)
								{
									//Error handle it
								}
								memset(szValue, '\0', l + 1);
								stringcopy(szValue, l + 1, chTempBuffer2);
								//strcpy_s(szValue, l + 1, chTempBuffer2);
								nReturnValue = AddNameValueToDictionary(dictHttpReq, szName, szValue);
								if (nReturnValue != 0)
								{
									//Error handle it
								}

							}
							memset(chTempBuffer2, '\0', (i - j) + 1);
							l = -1;
						}
						k++;
						l++;
						
					}
				}
			}

			free(chTempBuffer);
			free(chTempBuffer2);
			chTempBuffer = 0;
			chTempBuffer2 = 0;
			nLineNo++;
			j = i + 1;
		}
	}

	return nReturnValue;
}

char* GetFilePathFromURI(char *szURI, char *szLocalPath)
{

	int i = 0;
	int j = 0;
	int nLengthOfURI = 0;
	int nLengthOfLocalPath = 0;
	char* szFullPath = 0;

	if (szURI == 0)
	{
		return szFullPath;
	}

	nLengthOfLocalPath = strlen(szLocalPath);

	szFullPath = (char*)malloc(sizeof(char) * 260);
	if (szFullPath == 0)
		return szFullPath;

	memset(szFullPath, '\0', 260);

	if (szLocalPath[0] == '\"' && szLocalPath[nLengthOfLocalPath - 1] == '\"')
	{
		substringcopy(szFullPath, 1, nLengthOfLocalPath - 1, szLocalPath);
		szFullPath[nLengthOfLocalPath - 2] = '\0';
		nLengthOfLocalPath -= 2;

	}
	else
	{
		stringcopy(szFullPath, nLengthOfLocalPath, szLocalPath);
	}


	if (szLocalPath[nLengthOfLocalPath - 1] != '\\')
	{
		szFullPath[nLengthOfLocalPath] = '\\';
	}

	nLengthOfURI = strlen(szURI);
	j = nLengthOfLocalPath + 1;
	while (i < nLengthOfURI)
	{
		if (szURI[i] == '/')
		{
			if (j != nLengthOfLocalPath + 1)
			{
				szFullPath[j++] = '\\';
			}
		}
		else
		{
			szFullPath[j++] = szURI[i];
		}
		i++;
	}

	szFullPath[j] = '\0';

	return szFullPath;
}

int FindFileInLocalPath(char *szURI, char *szLocalPath)
{
	int nReturnValue = 0;
	int i = 0;
	int j = 0;
	int nLengthOfURI = 0;
	int nLengthOfLocalPath = 0;
	char* szFullPath = 0;

	if (szURI == 0)
	{
		return ERR_INVALID_URI;
	}
	
	

	szFullPath = GetFilePathFromURI(szURI, szLocalPath);
	
	if (szFullPath != 0)
	{
		nReturnValue = FileExists(szFullPath);
		free(szFullPath);
		szFullPath = 0;
	}
	else
	{
		//Error in path construction;
	}

	return nReturnValue;
}