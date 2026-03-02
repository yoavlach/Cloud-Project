#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <ws2tcpip.h>
#include <Windows.h>
#include "Packet.h"
#include "ConnectionHandler.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 2424
#define SERVER_IP "127.0.0.1"

using namespace std;

class Client
{
public:
	~Client();
	void connectToServer();
	void login();
	void signup();
	void sendFile();
	void receiveFile();
private:
	ConnectionHandler _connectionHandler;
	SOCKET _socket;
	Packet _p;
};