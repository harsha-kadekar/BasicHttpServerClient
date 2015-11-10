#include"HandleHTTPRequest.h"

int HandleClientHTTPRequest(char* szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	if (nRequestSize <= 0 || szHttpRequest == 0)
		return ERR_INVALID_PARAMETERS;

	//Handle Method
	nReturnValue = GetTypeOfMethod(szHttpRequest, nRequestSize);
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
			nReturnValue = Handle_Get(szHttpRequest, nRequestSize, scClientSocket);
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

	return nReturnValue;
}


int Handle_ExtMethods(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method

	return nReturnValue;
}

int Handle_Get(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char* szURI = 0;
	int nHttpVersion = 0;
	//TODO: Yet to handle this type of method

	szURI = GetURIOfRequest(szHttpRequest, nRequestSize, METHOD_GET);
	if (szURI != NULL)
	{
		printf_s("BHS:INFO:Requested URI: %s\n", szURI);
		nHttpVersion = GetHTTPVersionOfRequest(szHttpRequest, nRequestSize);
		if (nHttpVersion < 0 || nHttpVersion > 1)
		{
			//ERROR handle it
		}
		else
		{
			if(nHttpVersion == 0)
				printf_s("BHS:INFO:Request is of version 1.1\n");
			else
				printf_s("BHS:INFO:Request is of version 1.0\n");
		}
			
	}
	else
	{
		//ERROR handle it
	}

	if (szURI != NULL)
	{
		free(szURI);
		szURI = NULL;
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