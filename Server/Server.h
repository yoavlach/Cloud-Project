#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include "MessageHandler.h"

#define PORT 2424

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
	const string& getMsgPart(int& iterator, string& buffer, const string& msg);
	SOCKET _socket;
	MessageHandler _messageHandler;
	ConnectionHandler _connectionHandler;
};