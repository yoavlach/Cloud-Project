#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include "UserAuth.h"

using namespace std;

#define PORT 2424

enum MSG_CODES {
	LOG_IN = 100,
	SIGN_UP = 200,
	GET_FILE = 300,
	START_FILE_TRANSFER = 400,
	FILE_TRANSFER = 401,
	FINISH_FILE_TRANSFER = 402
};

typedef struct Packet {
	MSG_CODES msgCode;
	std::string username;
	std::string password;
	std::string data;
}Packet;

class Server
{
public:
	Server();
	~Server();
	void waitForClient();
	void acceptClient();
	void clientHandler();
	MSG_CODES& parseMsg(const string& msg);
private:
	SOCKET _socket;
	UserAuth* _userAuth;
};