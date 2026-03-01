#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
using namespace std;

class ConnectionHandler
{
public:
	void sendMessage(const char* msg);
	void receiveMessage(char* buffer);
	void setSocket(const SOCKET& socket);
private:
	SOCKET _socket;
};