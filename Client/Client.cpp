#include "Client.h"

//Client::~Client();
void Client::connectToServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET) 
    {
        cerr << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
    }
    else
    {
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(54000);
        if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) 
        {
            cerr << "Invalid address/ Address not supported." << endl;
            closesocket(_socket);
            WSACleanup();
        }
        else if (connect(_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            cerr << "Connection to server failed: " << WSAGetLastError() << endl;
            closesocket(_socket);
            WSACleanup();
        }
        else
        {
            cout << "Successfully connected to the server!" << endl;
            _connectionHandler.setSocket(_socket);
        }
    }
}
void Client::login()
{
    string username = "", password = "", msg = "";
    char* serverMsg = new char [MAX]
    bool connected = false;
    do
    {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        msg = to_string(LOGIN) + to_string(username.length()) + username + to_string(password.length()) + password;
        _connectionHandler.sendMessage(msg.c_str());
        _connectionHandler.receiveMessage(msg.c_str());
    } while (!connected);
}
//void Client::signup();
//void Client::sendFile();
//void Client::receiveFile();