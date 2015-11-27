#include"HandleHTTPRequest.h"

int HandleClientHTTPRequest(char* szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	Dictionary *dictHttpReq = 0;
	
	if (nRequestSize <= 0 || szHttpRequest == 0)
		return ERR_INVALID_PARAMETERS;

	dictHttpReq = CreateDictionary(10);
	if (dictHttpReq == 0)
	{
		return ERR_INVALID_MEMORY_OPERATION;
	}

	nReturnValue = GenerateHttpRequestDictionary(szHttpRequest, nRequestSize, dictHttpReq);
	if (nReturnValue != 0)
	{
		//Error has occured while adding elements to dictHttpReq
		return nReturnValue;
	}

	//Handle Method
	nReturnValue = GetTypeOfMethod(GetValueFromDictionary(dictHttpReq, "METHOD"));
	switch (nReturnValue)
	{
		case METHOD_OPTIONS:
		{
			//Handle Options
			nReturnValue = Handle_Options(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_GET:
		{
			//Handle Get
			printf_s("BHS:INFO:Got GET method\n");
			nReturnValue = Handle_Get(dictHttpReq, scClientSocket);
			break;
		}
		case METHOD_HEAD:
		{
			//Handle Head
			nReturnValue = Handle_Head(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_POST:
		{
			//Handle Post
			nReturnValue = Handle_Post(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_PUT:
		{
			//Handle Put
			nReturnValue = Handle_Put(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_DELETE:
		{
			//Handle Delete
			nReturnValue = Handle_Delete(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_TRACE:
		{
			//Handle Trace
			nReturnValue = Handle_Trace(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_CONNECT:
		{
			//Handle Connect
			nReturnValue = Handle_Connect(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		case METHOD_EXTEND_METHODS:
		{
			//Handle extended-methods
			nReturnValue = Handle_ExtMethods(szHttpRequest, nRequestSize, scClientSocket);
			break;
		}
		default:
		{
			break;
		}
	}
	

	/*nReturnValue = send(scClientSocket, sTestMessage, 35, 0);
	if (nReturnValue == SOCKET_ERROR)
	{
		printf_s("BHS:ERROR:send to client failed %d\n", WSAGetLastError());
		nReturnValue = ERR_SENDTOCLIENTFAILED;
		closesocket(scClientSocket);
		scClientSocket = INVALID_SOCKET;
		WSACleanup();
		return nReturnValue;
	}*/

	DeleteDictionary(dictHttpReq);
	dictHttpReq = 0;

	return nReturnValue;
}


int Handle_ExtMethods(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Get(Dictionary* dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char* szURI = 0;
	int nHttpVersion = 0;
	int bFoundFile = 0;
	//TODO: Yet to handle this type of method


	szURI = GetValueFromDictionary(dictHttpRequest, "URI");
	if (szURI != NULL)
	{
		//Based on the URI - search in the local path for the file. If found then send 200 else send 404 not found error
		bFoundFile = FindFileInLocalPath(szURI, strMappedLocalPath);
		if (bFoundFile == 0)
			printf_s("\nBHS:ERROR:Cound not find the file in the local path:%s\n", szURI);
		else
			printf_s("\nBHS:INFO:Able to find the file in the local path:%s\n", szURI);
			
	}
	else
	{
		//ERROR handle it
	}

	return nReturnValue;
}

int Handle_Head(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Options(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Delete(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Trace(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Post(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Put(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Connect(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}