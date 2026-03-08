#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Packet.h"
#include "UsersManager.h"
#include "ConnectionHandler.h"
#include <Windows.h>

using namespace std::filesystem;
using namespace std;

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
    void getListOfFiles();
private:
    string getMsgPart(int& iterator, string& buffer, const string& msg, int lenSize);
    string buildMsg(int msgCode, const string& data);
    string formatLen(const string& len, int bytes);
    bool sendCurrFileContent(const string& currFileContent);
    string extractFileName(const string& filePath);
    Packet _p;
    UsersManager _usersManager;
    SOCKET _socket;
    ConnectionHandler _connectionHandler;
    string _connectedUsername;
};