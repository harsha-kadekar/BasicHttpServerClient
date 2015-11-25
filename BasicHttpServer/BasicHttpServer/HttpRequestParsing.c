#include"HttpRequestParsing.h"


int stringcopy(char* strDestination, int nNoOfCharacters, char* strSource)
{
	int i = 0;

	while (i < nNoOfCharacters)
	{
		strDestination[i] = strSource[i];
		i++;
	}

	return i;
}

/*
Name: GetTypeOfMethod
Description: This method is responsible for finding the method of HTTP request.
Parameter: Starting point of the RecieveMsg[THis is may not be complete request]
nCount: Length of the request
return: Type of method - each encoded by a numeric value OPTIONS:0, GET:1, HEAD:2, POST:3, PUT:4, DELETE:5, TRACE:6, CONNECT:7, EXTENDED:8
*/
int GetTypeOfMethod(char* szRecieveMsg, int nCount)
{
	int nReturnValue = 0;
	char* szOption = "OPTIONS ";
	char* szGet = "GET ";
	char* szHead = "HEAD ";
	char* szPost = "POST ";
	char* szPut = "PUT ";
	char* szDelete = "DELETE ";
	char* szTrace = "TRACE ";
	char* szConnect = "CONNECT ";

	char szTempReq[9] = { '\0' };

	if (nCount < 3)
	{
		printf_s("BHS::ERROR: request message is less than 3 bytes\n");
		return ERR_REQUESTSIZELESS;
	}

	if (0 == szRecieveMsg)
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

	//TODO:: extenion - method - How to identify them?

	memset(szTempReq, '\0', 9);

	if (nCount >= 8)
	{
		stringcopy(szTempReq, 7, szRecieveMsg);
	}
	else if (nCount >= 7)
	{
		stringcopy(szTempReq, 6, szRecieveMsg);

	}
	else if (nCount >= 6)
	{
		stringcopy(szTempReq, 5, szRecieveMsg);

	}
	else if (nCount >= 5)
	{
		stringcopy(szTempReq, 4, szRecieveMsg);

	}
	else
	{
		if(nCount = 4)
			stringcopy(szTempReq, 3, szRecieveMsg);
		else
			stringcopy(szTempReq, 2, szRecieveMsg);

	}

	if (strncmp(szTempReq, szConnect, 8) == 0)
		nReturnValue = METHOD_CONNECT;
	else if (strncmp(szTempReq, szOption, 8) == 0)
		nReturnValue = METHOD_OPTIONS;
	else if (strncmp(szTempReq, szDelete, 7) == 0)
		nReturnValue = METHOD_DELETE;
	else if (strncmp(szTempReq, szTrace, 6) == 0)
		nReturnValue = METHOD_TRACE;
	else if (strncmp(szTempReq, szPost, 5) == 0)
		nReturnValue = METHOD_POST;
	else if (strncmp(szTempReq, szHead, 5) == 0)
		nReturnValue = METHOD_HEAD;
	else if (strncmp(szTempReq, szPut, 4) == 0)
		nReturnValue = METHOD_PUT;
	else if (strncmp(szTempReq, szGet, 4) == 0)
		nReturnValue = METHOD_GET;
	else
		nReturnValue = ERR_CANNOTIDENTIFYMETHOD;

	return nReturnValue;
}

/*
Name: GetURIOfRequest
Description: This function will get the URI sent in the HTTP request
Parameters: szRequest - actual http request,
			nCount - Length of the http request,
			nMethod - Type of method http request has used.
ReturnValue: Null terminated string - URI
*/
char* GetURIOfRequest(char* szRequest, int nCount, int nMethod)
{
	//"*" | absoluteURI | abs_path | authority
	//GET /pub/WWW/TheProject.html HTTP/1.1
	//GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1
	//OPTIONS * HTTP/1.1

	char* szURI = NULL;
	int bFoundFirstSpace = 0;
	int bFoundSecondSpace = 0;
	int i = 0;
	int nSize = 0;
	int nStartIndex = 0;
	
	if (nCount <= 0 || szRequest == 0)
		return szURI;

	while (i < nCount)
	{
		if (szRequest[i] == ' ' || szRequest[i] == '\t')
		{
			bFoundFirstSpace = 1;
			break;
		}
		else
		{
			i++;
		}
	}

	if (bFoundFirstSpace == 1)
	{
		nStartIndex = ++i;
		while (i < nCount)
		{
			if (szRequest[i] == ' ' || szRequest[i] == '\t')
			{
				bFoundSecondSpace = 1;
				break;
			}
			nSize++;
			i++;
		}

		if ((bFoundSecondSpace == 1) && nSize > 0)
		{
			szURI = (char*)malloc(sizeof(char)*(nSize+1));
			if (szURI == 0)
			{
				return szURI;
			}
			else
			{
				memset(szURI, '\0', nSize + 1);
				
				i = 0;
				while (i < nSize)
				{
szURI[i] = szRequest[i + nStartIndex];
i++;
				}

			}

		}
	}

	return szURI;
}

/*
Name: GetHTTPVersionOfRequest
Description: This function will return the version of the http request - is it HTTP 1.1 or HTTP 1.0
Parameters: szRequest - Actual http request
			nCount - Length of the http request
ReturnValue: 0 for HTTP 1.1, 1 for HTTP 1.0 else -ve error codes in case of error
*/
int GetHTTPVersionOfRequest(char *szRequest, int nCount)
{
	int nReturnValue = ERR_NOT_FOUND_HTTP_VER;
	int nSpaceCount = 0;
	int i = 0;
	char *szHttp1_1 = "HTTP/1.1";
	char *szHttp1_0 = "HTTP/1.0";
	int bFoundStart = 0;


	//GET /pub/WWW/TheProject.html HTTP/1.1
	//GET /pub/WWW/TheProject.html HTTP/1.0

	if (szRequest == 0 || nCount <= 0)
		return ERR_INVALID_REQUEST;

	while (i < nCount)
	{
		if (szRequest[i] == ' ' || szRequest[i] == '\t')
		{
			nSpaceCount++;
			if (nSpaceCount == 2)
			{
				bFoundStart = 1;
				i++;
				break;
			}
		}
		i++;
	}

	if (bFoundStart == 1)
	{
		if (strncmp(&szRequest[i], szHttp1_1, 7) == 0)
			nReturnValue = 0;
		else if (strncmp(&szRequest[i], szHttp1_0, 7) == 0)
			nReturnValue = 1;
		else
			nReturnValue = ERR_NOT_FOUND_HTTP_VER;

	}

	return nReturnValue;
}

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

							AddNameValueToDictionary(dictHttpReq, szName, szValue);
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
								AddNameValueToDictionary(dictHttpReq, szName, szValue);

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