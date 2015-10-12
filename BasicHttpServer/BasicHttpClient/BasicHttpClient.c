/*
Name: BasicHttpClient.c
Description: This file has functions for initial socket creation for client will happen. This file has the main entry point of application
Developer: Harsha Kadekar
Reference: 
*/

#include"BasicHttpClient.h"

#pragma comment (lib, "Ws2_32.lib")

char* sServerName = "GarudNextGen";
char* sTestMessage = "Hi, GarudNextGen; This is client1";
SOCKET scConnectSocket = INVALID_SOCKET;

/*
Name: main
Description: This is the main entry point of the application BasicHttpClient.exe
Parameter: argc - It will have only value 1 - Not used for application
		   argv - It will have arguments passed in command line - Not used for application
ReturnValue: 0 for success else error values
*/
int main(int argc, char** argv)
{
	int nReturnValue = 0;

	printf_s("BHC:INFO:Client has started.........\n");
	printf_s("BHC:INFO:Client is initializing.......\n");
	nReturnValue = InitializeSocket();
	if (nReturnValue != 0)
	{
		printf_s("BHC:ERROR:Error while initializing the client socket\n");

	}
	else
	{
		printf_s("BHC:INFO:Client has finsihed initializing......\n");
		printf_s("BHC:INFO:Connecting to server......\n");
		nReturnValue = DiscussWithServer();
		if(nReturnValue != 0)
			printf_s("BHC:ERROR:Error while discussing with server\n");
	}
	printf_s("BHS:INFO:exiting the client\n");

	return nReturnValue;
}

/*
Name: InitializeSocket
Description: This will create the socket which will be used by the client to do the connection with server
Parameters: -
ReturnValue: 0 for success else error values
*/
int InitializeSocket()
{
	int nReturnValue = 0;
	WSADATA wsData;

	struct addrinfo structClientaddr;
	struct addrinfo* structServeraddrInfo = NULL, *structAddrIterator = NULL;

	nReturnValue = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (nReturnValue != 0)
	{
		printf_s("BHC:ERROR:WSA startup failed %d\n", WSAGetLastError());
		nReturnValue = ERR_WSASTARTUPFAILED;
		return nReturnValue;
	}

	ZeroMemory(&structClientaddr, sizeof(structClientaddr));
	structClientaddr.ai_family = AF_INET;
	structClientaddr.ai_protocol = IPPROTO_TCP;
	structClientaddr.ai_socktype = SOCK_STREAM;

	nReturnValue = getaddrinfo(sServerName, "8080", &structClientaddr, &structServeraddrInfo);
	if (nReturnValue != 0)
	{
		printf_s("BHC:ERROR:Getting server address info failed %d\n", nReturnValue);
		nReturnValue = ERR_GETSERVERADDRFAIL;
		WSACleanup();
	}
	
	for (structAddrIterator = structServeraddrInfo; structAddrIterator != NULL; structAddrIterator = structServeraddrInfo->ai_next)
	{
		scConnectSocket = socket(structAddrIterator->ai_family, structAddrIterator->ai_socktype, structAddrIterator->ai_protocol);
		if (INVALID_SOCKET == scConnectSocket)
		{
			printf_s("BHC:ERROR:Socket creation failed %d\n", WSAGetLastError());
			WSACleanup();
			nReturnValue = ERR_SOCKETCREATIONFAIL;
			freeaddrinfo(structServeraddrInfo);
			return nReturnValue;
			
		}

		nReturnValue = connect(scConnectSocket, structAddrIterator->ai_addr, structAddrIterator->ai_addrlen);
		if (SOCKET_ERROR == nReturnValue)
		{
			closesocket(scConnectSocket);
			scConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;

	}

	freeaddrinfo(structServeraddrInfo);

	if (INVALID_SOCKET == scConnectSocket)
	{
		printf_s("BHC:ERROR:No valid server connection could be established\n");
		nReturnValue = ERR_SERVERCONNECTIONFAIL;
		WSACleanup();
		return nReturnValue;
	}
	

	return nReturnValue;
}

/*
Name: DiscussWithServer
Description: This function will send messages to server and also recieve messages from server.
Parameter: - 
ReturnValue: 0 for success else error codes.
*/
int DiscussWithServer()
{
	int nReturnValue = 0;
	char szRecieveMessage[512] = { '\0' };


	if (scConnectSocket == INVALID_SOCKET)
	{
		printf_s("BHC:ERROR:Invalid socket, cannot proceed\n");
		nReturnValue = ERR_INVALIDSOCKET;
		return nReturnValue;
	}

	nReturnValue = send(scConnectSocket, sTestMessage, strnlen_s(sTestMessage, 33), 0);
	if (nReturnValue == SOCKET_ERROR)
	{
		printf_s("BHC:ERROR:Not able to send message to server %d\n", WSAGetLastError());
		closesocket(scConnectSocket);
		scConnectSocket = INVALID_SOCKET;
		WSACleanup();
		nReturnValue = ERR_SERVERSENDERROR;
		return nReturnValue;
	}

	do
	{
		nReturnValue = recv(scConnectSocket, szRecieveMessage, 512, 0);
		if (nReturnValue > 0)
		{
			printf_s("BHS:INFO:Recieved %d bytes from server\n", nReturnValue);
			printf_s("BHS:INFO:Recieved message: %s", szRecieveMessage);
			nReturnValue = 0;
		}
		else if (nReturnValue == 0)
		{
			printf_s("BHS:INFO:Connection closed by server\n");
		}
		else
		{
			printf_s("BHS:ERROR:Recieve failed from server %d\n", WSAGetLastError());
			nReturnValue = ERR_SERVERRECVERROR;
		}

		memset(szRecieveMessage, '\0', 512);

	} while (nReturnValue > 0);

	closesocket(scConnectSocket);
	WSACleanup();

	return nReturnValue;
}

