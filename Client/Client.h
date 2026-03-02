#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <ws2tcpip.h>
#include <Windows.h>
#include <fstream>
#include <math.h>
#include "Packet.h"
#include "ConnectionHandler.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 2424
#define SERVER_IP "127.0.0.1"
#define MAX_SERVER_MESSAGE_LEN 1005
#define MAX_CLIENT_MESSAGE_LEN 1208

using namespace std;

class Client
{
public:
	void connectToServer();
	void login();
	void signup();
	void sendFile();
	void receiveFile();
private:
	const string& extractFileName(const string& filePath);
	const string& buildMsg(int msgCode, const string& username, const string& password, const string& data);
	const string& formatLen(const string& len, int bytes);
	ConnectionHandler _connectionHandler;
	SOCKET _socket;
	Packet _p;
};