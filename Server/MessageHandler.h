#pragma once
#include "Packet.h"
#include "UsersManager.h"
#include "ConnectionHandler.h"
#include <fstream>

#define MAX_CLIENT_MESSAGE_LEN 1208
using namespace std;

class MessageHandler
{
public:
    MessageHandler();
    Packet& parseMsg(const string& msg);
    void callMsgProcessFunc(const string& msg);
    void setPacket(const Packet& p);
    void setSocket(const SOCKET& socket);
    void login();
    void signup();
    void getFile();
    void sendFile();
private:
    const string& getMsgPart(int& iterator, string& buffer, const string& msg);
    const string& buildMsg(int msgCode, const string& data);
    const string& formatLen(const string& len, int bytes);
    Packet _p;
    UsersManager _usersManager;
    SOCKET _socket;
    ConnectionHandler _connectionHandler;
};