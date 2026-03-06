#include "Server.h"
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include "Windows.h"
#pragma comment(lib, "Ws2_32.lib")

Server::Server()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
        throw exception("WSAStartup failed with error: " + result);
    std::cout << "Winsock initialized." << std::endl;
    DWORD dwAttrib = GetFileAttributesA("usersData");
    BOOL dirExists = (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    if (!dirExists)
        CreateDirectoryA("usersData", NULL);
}

Server::~Server()
{
	try
	{
		closesocket(_socket);
	}
	catch (...) {}
}

void Server::waitForClient()
{
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET) 
    {
        WSACleanup();
        throw exception("Socket creation failed with error: " + WSAGetLastError());
    }
    std::cout << "Server socket created." << std::endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    serverAddress.sin_port = htons(PORT);      // Port number (htons converts to network byte order)

    if (bind(_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        closesocket(_socket);
        WSACleanup();
        throw exception("Bind failed with error: " + WSAGetLastError());
    }
    std::cout << "Socket bound to port " << PORT << "." << std::endl;

    if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(_socket);
        WSACleanup();
        throw exception("Listen failed with error: " + WSAGetLastError());
    }
    std::cout << "Listening for incoming connections..." << std::endl;
    acceptClient();
}
void Server::acceptClient()
{
    sockaddr_in clientAddress;
    int clientSize = sizeof(clientAddress);
    _socket = accept(_socket, (sockaddr*)&clientAddress, &clientSize);

    if (_socket == INVALID_SOCKET) 
    {
        closesocket(_socket);
        WSACleanup();
        throw exception("Accept failed with error: " + WSAGetLastError());
    }
    std::cout << "Client connected!" << std::endl;
    _connectionHandler.setSocket(_socket);
    _messageHandler.setSocket(_socket);
    clientHandler();
}

void Server::clientHandler()
{
	bool isConnected = true;
	char* clientMessage = new char[MAX_CLIENT_MESSAGE_LEN];
	while (isConnected)
	{
		try
		{
			_connectionHandler.receiveMessage(clientMessage);
			_messageHandler.callMsgProcessFunc(clientMessage);
		}
		catch (const exception& e)
		{
			isConnected = e.what() != "Connection closed by client";
			cout << e.what() << endl;
		}
	}
}