#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include "Packet.h"
using namespace std;

class ConnectionHandler
{
public:
	void sendMessage(const char* msg);
	void receiveMessage(char* buffer);
	void setSocket(const SOCKET& socket);
private:
	void sendAll(const char* data, int length);
	void receiveAll(char* buffer, int length);
	SOCKET _socket;
};