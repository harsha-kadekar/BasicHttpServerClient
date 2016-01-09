/*
Name: BasicHttpServer.c
Description: This file will have main entry point of the BasicHttpServer application. 
Developer: HarshaKadekar
References: 
*/

#include "BasicHttpServer.h"
#include "ConfigFileParser.h"

#pragma comment(lib,"ws2_32.lib") 

SOCKET scListenSocket = INVALID_SOCKET;

char* sTestMessage = "GarudNextGen: Recieved your message";

int nThread_Count = 0;
//int nMax_Thread_Count = 512;


CRITICAL_SECTION mcriticalThreadSync;
CONDITION_VARIABLE mcvThreadCount;

/*
Name: main
Description: this is the main entry point of the application. It will just call other functions to do the activity
Parameters: argc - Argument count - It will have only 1 - application name
			argv - values of the argument passed - it will have only application name
ReturnValue: 0 for success else error codes
*/
int main(int argc, char** argv)
{
	int nReturnValue = 0;
	char szMessage[1024] = { '\0' };

	printf("\nBHS:INFO:Started BASIC HTTP SERVER.....\n");
	printf_s("\nBHS:INFO:Started reading server configuraiton file\n");
	nReturnValue = ConfigFileParserInitialization();
	if (nReturnValue != 0)
	{
		printf_s("\nBHS:ERROR:Initialization of configuraition file reading failed with error:%d\n", nReturnValue);
		return nReturnValue;
	}
	nReturnValue = ReadConfigurationFile();
	if (nReturnValue != 0)
	{
		printf_s("\nBHS:ERROR:Error in reading configuraiton file = %d\n", nReturnValue);
		return nReturnValue;
	}
	nReturnValue = InitializeLogger();
	//printf("\nBHS:INFO:Initializing Socket......\n");
	if (nReturnValue != 0)
	{
		printf("\nBHS:ERROR:Initialization of logger failed... ending the server: %d\n", nReturnValue);
		//sprintf_s(szMessage, 1024, "Initialization of logger failed... ending the server: %d\n", nReturnValue);
		//LogMessage(LOG_ERROR, szMessage);
		UninitializeConfigurationParameters();
		return nReturnValue;
	}

	nReturnValue = InitializeSocket();
	if (nReturnValue != 0)
	{
		//printf_s("BHS:ERROR:Error has occured during initialization of socket\n");
		sprintf_s(szMessage, 1024, "BHS:ERROR:Error has occured during initialization of socket %d", nReturnValue);
		LogMessage(LOG_ERROR, szMessage);
	}
	else
	{
		//printf("\nBHS:INFO:Finished Socket initialization......\n");
		//printf("\nBHS:INFO:Started listening for client socket connection.......\n");
		nReturnValue = ListenForClientConnection();
		if (nReturnValue != 0)
		{
			printf_s("BHS:ERROR:ERROR has occurred during listening to client connection\n");
		}
	}

	printf_s("\nBHS:INFO:Uninitializing the parameters configured after reading configuration files\n");
	nReturnValue = UninitializeConfigurationParameters();
	if (nReturnValue != 0)
	{
		printf_s("\nBHS:ERROR:error during uninitializing the parameters: %d\n", nReturnValue);
	}
	printf("\nBHS:INFO:Exiting BASIC HTTP SERVER.......\n");

	nReturnValue = UninitializeLogger();

	return nReturnValue;
}

/*
Name: InitializeSocket
Description: This function will initialize the socket for initial starting of server. Then server will keep on waiting
			 under this socket.
Parameter: -
ReturnValue: 0 for success else error codes.
*/
int InitializeSocket()
{
	
	int nReturnValue = 0;

	struct addrinfo structServerAddrInfo, *structServerAddrResult = NULL;
	WSADATA wsData;
	char strPort[6] = { '\0' };
	char szMessage[1024] = { '\0' };

	LogMessage(LOG_DEBUG, "Initializing sockets");

	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0)
	{
		//printf_s("BHS:ERROR:Initialize of Socket failed with error code %d\n", WSAGetLastError());
		sprintf_s(szMessage, 1024, "Initialize of Socket failed with error code %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szMessage);
		nReturnValue = ERR_SOCKETINITIALIZEFAILED;
		return nReturnValue;
	}

	ZeroMemory(&structServerAddrInfo, sizeof(structServerAddrInfo));
	structServerAddrInfo.ai_family = AF_INET;
	structServerAddrInfo.ai_socktype = SOCK_STREAM;
	structServerAddrInfo.ai_protocol = IPPROTO_TCP;
	structServerAddrInfo.ai_flags = AI_PASSIVE;

	memset(strPort, '\0', 6);

	sprintf_s(strPort, 6, "%d", nConfiguredPortNumbered);
	
	LogMessage(LOG_DEBUG, "Getting the address info");
	nReturnValue = getaddrinfo(NULL, "8080", &structServerAddrInfo, &structServerAddrResult);
	if (nReturnValue != 0)
	{
		//printf_s("BHS:ERROR:Could not get address, got error %d\n", nReturnValue);
		sprintf_s(szMessage, 1024, "Could not get address, got error %d\n", nReturnValue);
		LogMessage(LOG_ERROR, szMessage);
		WSACleanup();
		nReturnValue = ERR_ADDRINFOFAILED;
		return nReturnValue;
	}

	LogMessage(LOG_DEBUG, "Creating the server socket");
	scListenSocket = socket(structServerAddrResult->ai_family, structServerAddrResult->ai_socktype, structServerAddrResult->ai_protocol);
	if (INVALID_SOCKET == scListenSocket)
	{
		//printf_s("BHS:ERROR:Could not create listen socket %d\n", WSAGetLastError());
		sprintf_s(szMessage, 1024, "Could not create listen socket %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szMessage);
		nReturnValue = ERR_LISTENSOCKETCREATEFAIL;
		freeaddrinfo(structServerAddrResult);
		structServerAddrResult = NULL;
		WSACleanup();
		return nReturnValue;
	}

	LogMessage(LOG_DEBUG, "Binding to the socket");
	nReturnValue = bind(scListenSocket, structServerAddrResult->ai_addr, structServerAddrResult->ai_addrlen);
	if (nReturnValue == SOCKET_ERROR)
	{
		//printf_s("BHS:ERROR:Error while binding the socket %d\n", WSAGetLastError());
		sprintf_s(szMessage, 1024, "Error while binding the socket %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szMessage);
		nReturnValue = ERR_BINDFAIL;
		freeaddrinfo(structServerAddrResult);
		structServerAddrResult = NULL;
		WSACleanup();
	}

	freeaddrinfo(structServerAddrResult);
	structServerAddrResult = NULL;

	LogMessage(LOG_DEBUG, "returning after initializing socket");
	return nReturnValue;
}

/*
Name: ListenForClientConnection
Description: This function will have an infinite loop where it will wait forever for client connections. Once client connection is established.
			 It will process it in a fast way or religate the work to a thread and keeps waiting for new connections.
Parameters: - 
ReturnValue: 0 for success else error codes.
*/
int ListenForClientConnection()
{
	int nReturnValue = 0;
	struct addrinfo *structClientAddrInfo = NULL;
	SOCKET scClientSocket = INVALID_SOCKET;
	HANDLE hClientThread = NULL;
	DWORD dwClientThreadId = 0;
	char szMessage[1024] = { '\0' };

	LogMessage(LOG_DEBUG, "Started to listen for client connection");

	if (INVALID_SOCKET == scListenSocket)
	{
		LogMessage(LOG_ERROR, "Invalid listen socket");
		nReturnValue = ERR_INVALIDLISTENSOCKET;
		return nReturnValue;
	}

	LogMessage(LOG_INFO, "listening.........");
	nReturnValue = listen(scListenSocket, SOMAXCONN);
	if (nReturnValue == SOCKET_ERROR)
	{
		//printf_s("BHS:ERROR:listen failed with error %d\n", WSAGetLastError());
		sprintf_s(szMessage, 1024, "listen failed with error %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szMessage);
		closesocket(scListenSocket);
		scListenSocket = INVALID_SOCKET;
		WSACleanup();
		nReturnValue = ERR_LISTENFAILURE;
		return nReturnValue;
	}

	LogMessage(LOG_INFO, "Initializing synchronization elements");

	InitializeCriticalSection(&mcriticalThreadSync);
	InitializeConditionVariable(&mcvThreadCount);

	LogMessage(LOG_INFO, "Going for infinite loop of accepts");
	while ((scClientSocket = accept(scListenSocket, NULL, NULL)) != INVALID_SOCKET)
	{

		//printf_s("BHS:INFO:Client connection established: \n");
		LogMessage(LOG_INFO, "Client connection established, Creating a thread to handle the connection");
		//printf_s("BHS:INFO:Creating a thread to handle the connection\n");
		
		hClientThread = CreateThread(NULL, 0, HandleClientRequestThread, scClientSocket, 0x00000004, &dwClientThreadId);

		EnterCriticalSection(&mcriticalThreadSync);


		if (nThread_Count >= nThreadPoolSize)
		{
			LogMessage(LOG_DEBUG, "Active thread count is more than thread pool size");
			SleepConditionVariableCS(&mcvThreadCount, &mcriticalThreadSync, INFINITE);
			LogMessage(LOG_DEBUG, "Got up from sleep(wait), now waking the creating thread to handle client request");
			
		}

		nThread_Count++;
		ResumeThread(hClientThread);

		LeaveCriticalSection(&mcriticalThreadSync);

		LogMessage(LOG_INFO, "Returning to wait for new client connections......");
		//printf_s("BHS:INFO:Returning to wait for new connections......\n");
	}

	if (scClientSocket == INVALID_SOCKET)
	{
		//printf_s("BHS:ERROR:Invalid client socket %d\n", WSAGetLastError());
		sprintf_s(szMessage, 1024, "Invalid client socket %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szMessage);
		nReturnValue = ERR_INVALIDCLIENTSOCKET;
	}
	else
	{
		LogMessage(LOG_INFO, "Closing the client socket");
		closesocket(scClientSocket);
		scClientSocket = INVALID_SOCKET;
	}
	
	LogMessage(LOG_INFO, "Closing the listen socket");
	closesocket(scListenSocket);
	scListenSocket = INVALID_SOCKET;
	WSACleanup();

	DeleteCriticalSection(&mcriticalThreadSync);
	
	LogMessage(LOG_DEBUG, "Finished listening for client connection");
	return nReturnValue;
}

/*
Name: HandleClientRequestThread
Description: This is the thread which will handle all the client request. Each client request will have a new thread.
Parameter: lpParam -> Client Socket.
ReturnValue: 0 for success else error.
*/
DWORD WINAPI HandleClientRequestThread(LPVOID lpParam)
{
	int nReturnValue = 0;
	int nRequestSize = 0;
	int nPrevSize = 0;
	int i = 0;
	SOCKET scClientSocket = (SOCKET)lpParam;
	//char szRecieveMsg[8192] = { '\0' };
	char *szRecieveMsg = 0;
	char *temp = NULL;
	char *szFullRequestMsg = 0;
	char* szErrorMessage = "HTTP/1.0 404 Not Found";
	boolean bFoundSecondSpace = 0;

	LogMessage(LOG_DEBUG, "Going to handle Client request thread");

	if (scClientSocket == INVALID_SOCKET)
	{
		LogMessage(LOG_ERROR, "Invalid client socket, ending handling of client socket");
		nReturnValue = ERR_INVALIDCLIENTSOCKET;
		return nReturnValue;
	}

	
	szRecieveMsg = (char*)malloc(sizeof(char)*nMaxClientRequestSize);
	memset(szRecieveMsg, '\0', nMaxClientRequestSize);

	LogMessage(LOG_INFO, "Starting to recieve client http request");
	while((nReturnValue = recv(scClientSocket, szRecieveMsg, nMaxClientRequestSize, 0)) > 0)
	{
		nPrevSize = nRequestSize;
		temp = szFullRequestMsg;
		//printf_s("BHS:INFO:Recieved %d bytes\n", nReturnValue);
		LogMessage(LOG_DEBUG, szRecieveMsg);
		//printf_s("BHS:INFO: %s\n", szRecieveMsg);

		nRequestSize = nReturnValue + nPrevSize;
		szFullRequestMsg = (char*)malloc(sizeof(char)*nRequestSize);
		if (szFullRequestMsg == 0)
		{
			//ERROR have to handle it
			LogMessage(LOG_ERROR, "MEMORY ALLOCATION FAILED FOR RECIEVE msg");
		}
		else
		{

			memset(szFullRequestMsg, '\0', nRequestSize);
			if (nPrevSize > 0)
			{
				strcpy_s(szFullRequestMsg, nPrevSize-1, temp);

				free(temp);
				temp = 0;
			}

			//strcpy_s(&szFullRequestMsg[nPrevSize], nReturnValue, szRecieveMsg);
			i = 0;
			while (i < nReturnValue-1)
			{
				szFullRequestMsg[i + nPrevSize] = szRecieveMsg[i];
				i++;
			}

			//\r\n\r\n
			LogMessage(LOG_DEBUG, "Verifying if end of http request is reached or not");
			if (strlen(szRecieveMsg)>4)
				if (szRecieveMsg[strlen(szRecieveMsg) - 1] == '\n' && szRecieveMsg[strlen(szRecieveMsg) - 2] == '\r' && szRecieveMsg[strlen(szRecieveMsg) - 3] == '\n' && szRecieveMsg[strlen(szRecieveMsg) - 4] == '\r')
					break;
		}

		

		
	}

	if (nReturnValue >= 0)
	{
		//printf_s("BHS:INFO:Client has closed socket\n");
		if (nReturnValue == 0)
			LogMessage(LOG_WARN, "Client has closed the connection");
		//printf_s("BHS:INFO:Going to handle client request\n");
		LogMessage(LOG_DEBUG, "Going to handle client request");

		nReturnValue = HandleClientHTTPRequest(szFullRequestMsg, nRequestSize, scClientSocket);
		if (nReturnValue != 0)
		{
			//ERROR HANDLE it
			sprintf_s(szErrorMessage, 1024, "Client HTTP request was not handled properly %d", nReturnValue);
			LogMessage(LOG_ERROR, szErrorMessage);
		}

		if (szFullRequestMsg != 0)
		{
			free(szFullRequestMsg);
			szFullRequestMsg = 0;
		}
	}
	else
	{
		//printf_s("BHS:ERROR:Could not recieve message from client %d\n", WSAGetLastError());
		sprintf_s(szErrorMessage, 1024, "Could not recieve message from client %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szErrorMessage);
		nReturnValue = ERR_RECIEVEFROMCLIENTFAILED;
		if (szRecieveMsg != 0)
		{
			free(szRecieveMsg);
			szRecieveMsg = 0;
		}
		LogMessage(LOG_INFO, "Closing client socket and returning");
		closesocket(scClientSocket);
		scClientSocket = INVALID_SOCKET;
		WSACleanup();
		return nReturnValue;
	}

	LogMessage(LOG_DEBUG, "Shutting down the socket");
	nReturnValue = shutdown(scClientSocket, SD_SEND);
	if (nReturnValue == SOCKET_ERROR)
	{
		//printf_s("BHS:ERROR:Could not shutdown client connection properly %d\n", WSAGetLastError());
		sprintf_s(szErrorMessage, 1024, "Could not shutdown client connection properly %d\n", WSAGetLastError());
		LogMessage(LOG_ERROR, szErrorMessage);
		nReturnValue = ERR_SHUTDOWNSOCKETERROR;
		if (szRecieveMsg != 0)
		{
			free(szRecieveMsg);
			szRecieveMsg = 0;
		}
		LogMessage(LOG_INFO, "Closing socket, and returning");
		closesocket(scClientSocket);
		scClientSocket = INVALID_SOCKET;
		WSACleanup();
		return nReturnValue;
	}

	LogMessage(LOG_DEBUG, "Closing client socket");
	closesocket(scClientSocket);
	scClientSocket = INVALID_SOCKET;
	//WSACleanup();

	EnterCriticalSection(&mcriticalThreadSync);
	nThread_Count--;
	WakeConditionVariable(&mcvThreadCount);
	LeaveCriticalSection(&mcriticalThreadSync);

	if (szRecieveMsg != 0)
	{
		free(szRecieveMsg);
		szRecieveMsg = 0;
	}

	LogMessage(LOG_DEBUG, "Returning from thread, finished handling client request");
	return nReturnValue;

}