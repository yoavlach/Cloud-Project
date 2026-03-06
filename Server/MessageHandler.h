#pragma once
#include <fstream>
#include "Packet.h"
#include "UsersManager.h"
#include "ConnectionHandler.h"

#define MAX_CLIENT_MESSAGE_LEN 1208
using namespace std;

class MessageHandler
{
public:
    Packet& parseMsg(const string& msg);
    void callMsgProcessFunc(const string& msg);
    void setPacket(const Packet& p);
    void setSocket(const SOCKET& socket);
    void setConnectedUsername(const string& connectedUsername);
    void login();
    void signup();
    void getFile();
    void sendFile();
private:
    string getMsgPart(int& iterator, string& buffer, const string& msg, int lenSize);
    string buildMsg(int msgCode, const string& data);
    string formatLen(const string& len, int bytes);
    Packet _p;
    UsersManager _usersManager;
    SOCKET _socket;
    ConnectionHandler _connectionHandler;
    string _connectedUsername;
};