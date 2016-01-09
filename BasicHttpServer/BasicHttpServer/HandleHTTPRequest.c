#include"HandleHTTPRequest.h"

int HandleClientHTTPRequest(char* szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	Dictionary *dictHttpReq = 0;
	
	LogMessage(LOG_DEBUG, "Started handling client http request");

	if (nRequestSize <= 0 || szHttpRequest == 0)
	{
		LogMessage(LOG_ERROR, "Invalid http request");
		return ERR_INVALID_PARAMETERS;
	}

	LogMessage(LOG_DEBUG, "Creating the dictionary for http request");
	dictHttpReq = CreateDictionary(10);
	if (dictHttpReq == 0)
	{
		LogMessage(LOG_ERROR, "Failed to create dictionary, returning");
		return ERR_INVALID_MEMORY_OPERATION;
	}

	LogMessage(LOG_DEBUG, "Populate the dictionary from http request");
	nReturnValue = GenerateHttpRequestDictionary(szHttpRequest, nRequestSize, dictHttpReq);
	if (nReturnValue != 0)
	{
		//Error has occured while adding elements to dictHttpReq
		LogMessage(LOG_ERROR, "Failed to add elements to the http request dictionary");
		return nReturnValue;
	}

	//Handle Method
	LogMessage(LOG_DEBUG, "Get the type of http request method");
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
			//printf_s("BHS:INFO:Got GET method\n");
			nReturnValue = Handle_Get(dictHttpReq, scClientSocket);
			break;
		}
		case METHOD_HEAD:
		{
			//Handle Head
			nReturnValue = Handle_Head(dictHttpReq, scClientSocket);
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
			nReturnValue = Handle_Delete(dictHttpReq, scClientSocket);
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
			LogMessage(LOG_ERROR, "Invalid type of method");
			//Handle the error
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
	//closesocket(scClientSocket);
	//WSACleanup();

	LogMessage(LOG_DEBUG, "Delete the created http request dictionary");
	DeleteDictionary(dictHttpReq);
	dictHttpReq = 0;

	LogMessage(LOG_DEBUG, "Returning after handling client http request");
	return nReturnValue;
}


int Handle_ExtMethods(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	LogMessage(LOG_DEBUG, "Going to handle the extended methods");
	//TODO: Yet to handle this type of method
	LogMessage(LOG_DEBUG, "Returning after handling the extended methods");
	return nReturnValue;
}

int Handle_Get(Dictionary* dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char* szURI = 0;
	int nHttpVersion = 0;
	int bFoundFile = 0;
	char* szQueryString = 0;
	char* szPathOfFile = 0;
	char szIndex = "index.html";
	char szPathAfterAdding[260] = { '\0' };
	//TODO: Yet to handle this type of method

	LogMessage(LOG_DEBUG, "Going to handle the Get type of method");

	szURI = GetValueFromDictionary(dictHttpRequest, "URI");
	if (szURI != NULL)
	{
		//Check if the uri has query string. If so then it needs to be handled by cgi.

		//Based on the URI - search in the local path for the file. If found then send 200 else send 404 not found error
		LogMessage(LOG_DEBUG, "Got the URI from the request");
		LogMessage(LOG_DEBUG, szURI);
		szQueryString = szURI;
		while ((*szQueryString) != '?' && (*szQueryString) != '\0')
		{
			szQueryString++;
		}
		if ((*szQueryString) == '?')
		{
			//This is a query string so handle it in a different way.
			LogMessage(LOG_DEBUG, "Got a query string in uri");

			szQueryString++;
			LogMessage(LOG_DEBUG, szQueryString);
		}
		else
		{
			LogMessage(LOG_DEBUG, "Query string not found");
			bFoundFile = FindFileInLocalPath(szURI, strMappedLocalPath);
			if (bFoundFile == 0)
			{
				//printf_s("\nBHS:ERROR:Cound not find the file in the local path:%s\n", szURI);
				LogMessage(LOG_ERROR, "Could not find the file in the local path");
				nReturnValue = HandleFileNotFound(dictHttpRequest, scClientSocket);
			}
			else
			{
				//printf_s("\nBHS:INFO:Able to find the file in the local path:%s\n", szURI);
				LogMessage(LOG_DEBUG, "Able to find the file in local path");
				szPathOfFile = GetFilePathFromURI(szURI, strMappedLocalPath);
				LogMessage(LOG_DEBUG, szPathOfFile);
				if (szPathOfFile[strnlen_s(szPathOfFile, 260) - 1] == '/')
				{
					//Add index.html file
					LogMessage(LOG_DEBUG, "Its a folder so adding index.html by default");
					stringcopy(szPathAfterAdding, strlen(szPathOfFile), szPathOfFile);
					strcat_s(szPathAfterAdding, 260, szIndex);
					free(szPathOfFile);
					if (FileExists(szPathAfterAdding) == 1)
					{
						szPathOfFile = (char*)malloc(sizeof(char)*strlen(szPathAfterAdding) + 1);
						memset(szPathOfFile, '\0', strlen(szPathAfterAdding)+1);
						stringcopy(szPathOfFile, strlen(szPathAfterAdding), szPathAfterAdding);
						LogMessage(LOG_DEBUG, "Going to send a http response now");
						LogMessage(LOG_DEBUG, szPathOfFile);
						nReturnValue = HandleGetFileResponse(dictHttpRequest, scClientSocket, szPathOfFile);

						free(szPathOfFile);
						szPathOfFile = 0;
					}
					else
					{
						LogMessage(LOG_DEBUG, "Could not find the index.html file inside the folder");
						LogMessage(LOG_DEBUG, szPathAfterAdding);
						nReturnValue = HandleFileNotFound(dictHttpRequest, scClientSocket);
					}

				}
				else
				{
					LogMessage(LOG_DEBUG, "About to handle the Get request of file");
					LogMessage(LOG_DEBUG, szPathOfFile);
					nReturnValue = HandleGetFileResponse(dictHttpRequest, scClientSocket, szPathOfFile);

					free(szPathOfFile);
					szPathOfFile = 0;
				}

			}
		}

		
			
	}
	else
	{
		//ERROR handle it
		LogMessage(LOG_ERROR, "URI was empty");
	}

	LogMessage(LOG_DEBUG, "Returning after handling GET request");
	return nReturnValue;
}

int Handle_Head(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char *szURI = 0;
	char *szQueryString = 0;
	char* szPathOfFile = 0;
	char szIndex = "index.html";
	char szPathAfterAdding[260] = { '\0' };
	int bFoundFile = 0;
	
	LogMessage(LOG_DEBUG, "Started to hanlde head http request");

	szURI = GetValueFromDictionary(dictHttpRequest, "URI");
	if (NULL != szURI)
	{
		//Check if the uri has query string. If so then it needs to be handled by cgi.

		//Based on the URI - search in the local path for the file. If found then send 200 else send 404 not found error

		LogMessage(LOG_DEBUG, "Got URI of request");
		LogMessage(LOG_DEBUG, szURI);

		szQueryString = szURI;
		while ((*szQueryString) != '?' && (*szQueryString) != '\0')
		{
			szQueryString++;
		}
		if ((*szQueryString) == '?')
		{
			//This is a query string so handle it in a different way.
			szQueryString++;
			LogMessage(LOG_DEBUG, "URI has a query string in it");
			LogMessage(LOG_DEBUG, szQueryString);
		}
		else
		{
			LogMessage(LOG_DEBUG, "request does not have query string");
			bFoundFile = FindFileInLocalPath(szURI, strMappedLocalPath);
			if (bFoundFile == 0)
			{
				LogMessage(LOG_ERROR, "Could not find the file in the local path");
				nReturnValue = HandleFileNotFound(dictHttpRequest, scClientSocket);
			}
			else
			{
				//printf_s("\nBHS:INFO:Able to find the file in the local path:%s\n", szURI);
				LogMessage(LOG_DEBUG, "Able to find the file in the local path");
				szPathOfFile = GetFilePathFromURI(szURI, strMappedLocalPath);
				LogMessage(LOG_DEBUG, szPathOfFile);
				if (szPathOfFile[strnlen_s(szPathOfFile, 260) - 1] == '/')
				{
					//Add index.html file
					stringcopy(szPathAfterAdding, strlen(szPathOfFile), szPathOfFile);
					strcat_s(szPathAfterAdding, 260, szIndex);
					free(szPathOfFile);
					if (FileExists(szPathAfterAdding) == 1)
					{
						LogMessage(LOG_DEBUG, "Path is directory, so added index.html to path");
						
						szPathOfFile = (char*)malloc(sizeof(char)*strlen(szPathAfterAdding) + 1);
						memset(szPathOfFile, '\0', strlen(szPathAfterAdding) + 1);
						stringcopy(szPathOfFile, strlen(szPathAfterAdding), szPathAfterAdding);
						LogMessage(LOG_DEBUG, szPathOfFile);
						nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);

						free(szPathOfFile);
						szPathOfFile = 0;
					}
					else
					{
						LogMessage(LOG_DEBUG, "Found to be directory, so added index.html, But file not in local path");
						LogMessage(LOG_DEBUG, szPathOfFile);
						nReturnValue = HandleHeadFileNotFound(dictHttpRequest, scClientSocket);
					}

				}
				else
				{
					LogMessage(LOG_DEBUG, "Going to send the head response");
					nReturnValue = HandleHeadFileResponse(dictHttpRequest, scClientSocket);

					free(szPathOfFile);
					szPathOfFile = 0;
				}

			}
		}
	}

	LogMessage(LOG_DEBUG, "Returning after handling head request");
	return nReturnValue;
}

int Handle_Options(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method
	LogMessage(LOG_DEBUG, "Starting to handle Options method of request");
	LogMessage(LOG_DEBUG, "Returning after handling handle options");
	return nReturnValue;
}

int Handle_Delete(Dictionary *dictHttpRequest, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	char *szURI = 0;
	char *szQueryString = 0;
	char* szPathOfFile = 0;
	char szIndex = "index.html";
	char szPathAfterAdding[260] = { '\0' };
	int bFoundFile = 0;

	LogMessage(LOG_DEBUG, "Starting to handle the delete method of request");

	szURI = GetValueFromDictionary(dictHttpRequest, "URI");
	if (szURI != 0)
	{
		LogMessage(LOG_DEBUG, "Got the URI");
		LogMessage(LOG_DEBUG, szURI);
		szQueryString = szURI;
		while ((*szQueryString) != '?' && (*szQueryString) != '\0')
		{
			szQueryString++;
		}

		if ((*szQueryString) == '?')
		{
			//This is a query string so handle it in a different way.
			szQueryString++;
			LogMessage(LOG_ERROR, "Got a query stirng, so its an invalid request");
			//Basically its a wrong HTTP request for this method...
		}
		else
		{

			bFoundFile = FindFileInLocalPath(szURI, strMappedLocalPath);
			if (bFoundFile == 0)
			{
				LogMessage(LOG_ERROR, "Given file could not be found");
				//printf_s("\nBHS:ERROR:Cound not find the file in the local path:%s\n", szURI);
				nReturnValue = HandleFileNotFound(dictHttpRequest, scClientSocket);
			}
			else
			{
				//printf_s("\nBHS:INFO:Able to find the file in the local path:%s\n", szURI);
				szPathOfFile = GetFilePathFromURI(szURI, strMappedLocalPath);
				LogMessage(LOG_DEBUG, "Able to map the uri to local path file");
				LogMessage(LOG_DEBUG, szPathOfFile);
				if (szPathOfFile[strnlen_s(szPathOfFile, 260) - 1] == '/')
				{
					//Not going to handle folder delete

				}
				else
				{

					nReturnValue = DeleteFileA(szPathOfFile);

					LogMessage(LOG_DEBUG, "Deleted the file, now going to send response");
					nReturnValue = HandleDeleteFileResponse(dictHttpRequest, scClientSocket, szPathOfFile);

					free(szPathOfFile);
					szPathOfFile = 0;
				}
			}
		}
	}
	else
	{
		//Error have to handle it.
		LogMessage(LOG_ERROR, "URI was invlaid");
	}

	LogMessage(LOG_DEBUG, "Returning after handling delete method");
	return nReturnValue;
}

int Handle_Trace(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method
	LogMessage(LOG_DEBUG, "Handle the trace method of http request");
	LogMessage(LOG_DEBUG, "Returning after handling trace method");
	return nReturnValue;
}

int Handle_Post(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method
	LogMessage(LOG_DEBUG, "started handling Post method of http request");
	LogMessage(LOG_DEBUG, "returning after handling post method of http request");
	return nReturnValue;
}

int Handle_Put(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method
	LogMessage(LOG_DEBUG, "Started handling put method of http request");
	LogMessage(LOG_DEBUG, "returning after finishing with Put");

	return nReturnValue;
}

int Handle_Connect(char *szHttpRequest, int nRequestSize, SOCKET scClientSocket)
{
	int nReturnValue = 0;
	//TODO: Yet to handle this type of method
	LogMessage(LOG_DEBUG, "Started handling connect method");
	LogMessage(LOG_DEBUG, "returning after handling connect method");
	return nReturnValue;
}