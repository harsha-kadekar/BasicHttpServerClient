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