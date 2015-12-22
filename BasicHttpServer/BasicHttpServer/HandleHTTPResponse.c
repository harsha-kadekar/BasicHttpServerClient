#include "HandleHTTPResponse.h"



char* GetLocalPath()
{

}

int HandleBadRequest(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	return nReturnValue;
}

int HandleFileNotFound(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;

	char* szHTTPResponseHeader = "HTTP/1.0 404 OK\r\nSERVER:basichttpse/0.0.1\r\nContent-Type: text/html\r\n";
	char* szFileContent = "<html><head><title>Page Not Found</title></head><body><p>Requested URI not found</p></body></html>";
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

int HandleHeadResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char *strFullPathOfFile = 0;
	int bResourseFound = 0;
	Dictionary *dictHttpResponse = 0;
	char *strName = 0;
	char *strValue = 0;

	/*dictHttpResponse = CreateDictionary(10);

	strName = (char*)malloc(sizeof(char)*strlen("HTTPVERSION")+1);
	memset(strName, '\0', strlen("HTTPVERSION") + 1);
	stringcopy(strName, strlen("HTTPVERSION"), "HTTPVERSION");
	AddNameValueToDictionary(dictHttpResponse, strName, GetValueFromDictionary(dictHttpRequest, "HTTPVERSION"));

	strFullPathOfFile = GetFullPathOfFile(strMappedLocalPath, GetValueFromDictionary(dictHttpRequest, "URI"));

	bResourseFound = PathFileExistsA(strFullPathOfFile);*/




	return nReturnValue;
}

int HandleGetFileResponse(Dictionary *dictHttpRequest, SOCKET scClientSocket, char *strFileToSend)
{
	int nReturnValue = 0;
	FILE* fpToSend = 0;
	errno_t err = 0;
	char* szHTTPResponseHeader = "HTTP/1.0 200 OK\r\nSERVER:basichttpse/0.0.1\r\nContent-Type: text/html\r\n";
	char szFileContent[8098] = { '\0' };

	nReturnValue = send(scClientSocket, szHTTPResponseHeader, strlen(szHTTPResponseHeader), 0);

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