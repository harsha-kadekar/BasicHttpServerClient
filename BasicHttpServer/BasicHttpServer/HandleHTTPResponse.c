#include "HandleHTTPResponse.h"



char* GetLocalPath()
{

}

int HandleBadRequest(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	return nReturnValue;
}

int HandleHeadFileNotFound(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char* szHTTPResponseHeader = "HTTP/1.0 404 OK\r\nSERVER:basichttpse/0.0.1\r\nContent-Type: text/html\r\n";
	nReturnValue = send(scClientSocket, szHTTPResponseHeader, strlen(szHTTPResponseHeader), 0);

	return nReturnValue;

}

int HandleFileNotFound(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);
	char* szFileContent = "<html><head><title>Page Not Found</title></head><body><p>Requested URI not found</p></body></html>";
	nReturnValue = send(scClientSocket, szFileContent, strlen(szFileContent), 0);

	return nReturnValue;
}

int HandleDeleteFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char* szFilePath)
{
	int nReturnValue = 0;

	nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);
	char* szFileContent = "<html><head><title>Resource Deleted</title></head><body><p>Requested URI has been deleted</p></body></html>";

	nReturnValue = send(scClientSocket, szFileContent, strlen(szFileContent), 0);

	return nReturnValue;
}

int HandleMethodNotImplemented(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	return nReturnValue;
}


int HandleFileFound(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;



	return nReturnValue;
}

int HandleForbiddenRequest(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	return nReturnValue;
}

int HandleHeadFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char* szHTTPResponseHeader = "HTTP/1.0 200 OK\r\nSERVER:basichttpse/0.0.1\r\nContent-Type: text/html\r\n";
	nReturnValue = send(scClientSocket, szHTTPResponseHeader, strlen(szHTTPResponseHeader), 0);

	return nReturnValue;
}

int HandleGetFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char *strFileToSend)
{
	int nReturnValue = 0;
	FILE* fpToSend = 0;
	errno_t err = 0;
	
	char szFileContent[8098] = { '\0' };

	nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);

	err = fopen_s(&fpToSend, strFileToSend, "r");
	if (err == 0)
	{
		while (fgets(szFileContent, 8097, fpToSend))
		{
			nReturnValue = send(scClientSocket, szFileContent, strlen(szFileContent), 0);
			memset(szFileContent, '\0', 8098);
		}
	}

	fclose(fpToSend);
	fpToSend = 0;

	return nReturnValue;
}