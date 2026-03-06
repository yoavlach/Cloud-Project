#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <string>
#include <iostream>
#include "MessageHandler.h"

#define PORT 54000

using namespace std;

class Server
{
public:
	Server();
	~Server();
	void waitForClient();
	void acceptClient();
	void clientHandler();
private:
	string getMsgPart(int& iterator, string& buffer, const string& msg);
	SOCKET _socket;
	MessageHandler _messageHandler;
	ConnectionHandler _connectionHandler;
};