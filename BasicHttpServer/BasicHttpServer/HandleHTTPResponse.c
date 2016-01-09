#include "HandleHTTPResponse.h"

int HandleBadRequest(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	return nReturnValue;
}

int HandleHeadFileNotFound(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char szErrorMsg[100] = { '\0' };
	char* szHTTPResponseHeader = "HTTP/1.0 404 OK\r\nSERVER:basichttpse/0.0.1\r\nContent-Type: text/html\r\n";
	LogMessage(LOG_DEBUG, szHTTPResponseHeader);
	nReturnValue = send(scClientSocket, szHTTPResponseHeader, strlen(szHTTPResponseHeader), 0);
	if (nReturnValue == SOCKET_ERROR)
	{
		sprintf_s(szErrorMsg, 100, "Error in sending response to client %d", WSAGetLastError());
		LogMessage(LOG_ERROR, szErrorMsg);
	}
	else
		nReturnValue = 0;

	return nReturnValue;

}

int HandleFileNotFound(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char szErrorMsg[100] = { '\0' };

	nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);
	if (nReturnValue == 0)
	{
		char* szFileContent = "<html><head><title>Page Not Found</title></head><body><p>Requested URI not found</p></body></html>";
		LogMessage(LOG_DEBUG, szFileContent);
		nReturnValue = send(scClientSocket, szFileContent, strlen(szFileContent), 0);
		if (nReturnValue == SOCKET_ERROR)
		{
			sprintf_s(szErrorMsg, 100, "Error in sending response to client %d", WSAGetLastError());
			LogMessage(LOG_ERROR, szErrorMsg);
		}
		else
			nReturnValue = 0;
	}

	return nReturnValue;
}

int HandleDeleteFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char* szFilePath)
{
	int nReturnValue = 0;
	char* szFileContent = "<html><head><title>Resource Deleted</title></head><body><p>Requested URI has been deleted</p></body></html>";
	char szErrorMsg[100] = { '\0' };

	nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);
	if (nReturnValue == 0)
	{
		LogMessage(LOG_DEBUG, szFileContent);
		nReturnValue = send(scClientSocket, szFileContent, strlen(szFileContent), 0);
		if (nReturnValue == SOCKET_ERROR)
		{
			sprintf_s(szErrorMsg, "Error while sending response to client %d", WSAGetLastError());
			LogMessage(LOG_ERROR, szErrorMsg);
		}
		else
			nReturnValue = 0;
	}
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
	char szErrorMsg[100] = { '\0' };

	LogMessage(LOG_DEBUG, szHTTPResponseHeader);
	nReturnValue = send(scClientSocket, szHTTPResponseHeader, strlen(szHTTPResponseHeader), 0);
	if (nReturnValue == SOCKET_ERROR)
	{
		sprintf_s(szErrorMsg, "Error while sending response to client %d", WSAGetLastError());
		LogMessage(LOG_ERROR, szErrorMsg);
	}
	else
		nReturnValue = 0;

	return nReturnValue;
}

int HandleGetFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char *strFileToSend)
{
	int nReturnValue = 0;
	FILE* fpToSend = 0;
	errno_t err = 0;
	
	char szFileContent[8098] = { '\0' };
	char szErrorMsg[100] = { '\0' };

	nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);
	if (nReturnValue == 0)
	{
		err = fopen_s(&fpToSend, strFileToSend, "r");
		if (err == 0)
		{
			while (fgets(szFileContent, 8097, fpToSend))
			{
				nReturnValue = send(scClientSocket, szFileContent, strlen(szFileContent), 0);
				if (nReturnValue == SOCKET_ERROR)
				{
					sprintf_s(szErrorMsg, 100, "Error while sending file to client %d", WSAGetLastError());
					LogMessage(LOG_ERROR, szErrorMsg);
					break;
				}
				else
				{
					nReturnValue = 0;
				}
				memset(szFileContent, '\0', 8098);
			}
		}
		else
		{
			sprintf_s(szErrorMsg, 100, "Error while opening the file to be sent to client %d", err);
			LogMessage(LOG_ERROR, szErrorMsg);
			nReturnValue = err;
		}

		if (fpToSend != 0)
		{
			fclose(fpToSend);
			fpToSend = 0;
		}
	}
	return nReturnValue;
}