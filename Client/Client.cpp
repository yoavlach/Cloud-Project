#include "Client.h"
void Client::connectToServer()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
        throw exception("WSAStartup failed: " + result);
    cout << "Winsock initialized." << std::endl;
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
        serverAddress.sin_port = htons(PORT);
        if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) 
        {
            closesocket(_socket);
            WSACleanup();
            throw exception("Invalid address/ Address not supported.");
        }
        else if (connect(_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) 
        {
            closesocket(_socket);
            WSACleanup();
            throw exception("Connection to server failed");
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
    char* serverMsg = new char[MAX_SERVER_MESSAGE_LEN];
    char* chMsg = nullptr;
    bool connected = false;
    do
    {
        try
        {
            cout << "Enter username: ";
            cin >> username;
            if (username.length() > 99)
                throw exception("Username length must be lower than 100");
            cout << "Enter password: ";
            cin >> password;
            if (password.length() > 99)
                throw exception("Username length must be lower than 100");
            msg = to_string(LOGIN) + to_string(username.length()) + username + to_string(password.length()) + password;
            _connectionHandler.sendMessage(msg.c_str());
            chMsg = new char[msg.length()];
            for (int i = 0; i < msg.length(); i++)
                chMsg[i] = msg[i];
            _connectionHandler.receiveMessage(chMsg);
            _p = _connectionHandler.parseMsg(chMsg);
            delete[] chMsg;
            switch (_p.msgCode)
            {
            case USER_DOESNT_EXIST:
                cout << "User doesn't exist\nPlease try again" << endl;
                break;
            case INCORRECT_PASSWORD:
                cout << "Incorrect password\nPlease try again" << endl;
                break;
            case LOGIN_SUCCESSFUL:
                cout << "Login successful" << endl;
                connected = true;
                break;
            }
        }
        catch (const exception& e)
        {
            cout << e.what() << endl;
        }
    } while (!connected);
    delete[] serverMsg;
}
void Client::signup()
{
    string username = "", password = "", msg = "";
    char* serverMsg = new char[MAX_SERVER_MESSAGE_LEN];
    char* chMsg = nullptr;
    bool connected = false;
    do
    {
        try
        {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            _connectionHandler.sendMessage(buildMsg(SIGNUP, username, password, "").c_str());
            chMsg = new char[msg.length()];
            for (int i = 0; i < msg.length(); i++)
                chMsg[i] = msg[i];
            _connectionHandler.receiveMessage(chMsg);
            _p = _connectionHandler.parseMsg(chMsg);
            delete[] chMsg;
            switch (_p.msgCode)
            {
            case USERNAME_ALREADY_EXISTS:
                cout << "Username already exists\nPlease try again" << endl;
                break;
            case SIGN_UP_SUCCESSFUL:
                cout << "Signup successful" << endl;
                connected = true;
                break;
            }
        }
        catch (const exception& e)
        {
            cout << e.what() << endl;
        }
    } while (!connected);
    delete[] serverMsg;
}
void Client::sendFile()
{
    string path = "", currLine = "", currFileContent = "", msg = "", fileName = "";
    char* serverMsg = new char[MAX_SERVER_MESSAGE_LEN];
    bool processSuccessful = false;
    ifstream f;
    do
    {
        cout << "Enter file path: ";
        cin >> path;
        f.open(path.c_str());
        processSuccessful = f.good();
        if (!processSuccessful)
            cout << "File doesn't exist" << endl;
    } while (!processSuccessful);
    fileName = extractFileName(path);
    try
    {
        _connectionHandler.sendMessage(buildMsg(READY_TO_SEND_FILE, "", "", fileName).c_str());
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }
    while (getline(f, currLine) && processSuccessful)
    {
        if (currFileContent.length() + currLine.length() >= 999)
        {
            msg = to_string(FILE_DATA) + to_string(currFileContent.length()) + currFileContent;
            try
            {
                _connectionHandler.sendMessage(msg.c_str());
                _connectionHandler.receiveMessage(serverMsg);
                _p = _connectionHandler.parseMsg(serverMsg);
                processSuccessful = _p.msgCode == FILE_DATA_RECEIVED;
            }
            catch (const exception& e)
            {
                processSuccessful = false;
                cout << e.what() << endl;
            }
        }
    }
    if (processSuccessful)
    {
        try
        {
            _connectionHandler.sendMessage(to_string(FINISHED_SENDING_FILE).c_str());
        }
        catch (const exception& e)
        {
            cout << e.what();
        }
    }
    f.close();
    delete[] serverMsg;
}
void Client::receiveFile()
{
    string fileName = "", fileContent = "", pathToSaveFile = "";
    char* serverMsg = new char[MAX_SERVER_MESSAGE_LEN];
    bool processSuccessful = false;
    ofstream f;
    do
    {
        try
        {
            cout << "Enter the file name: ";
            cin >> fileName;
            _connectionHandler.sendMessage(buildMsg(READY_TO_RECEIVE_FILE, "", "", fileName).c_str());
            _connectionHandler.receiveMessage(serverMsg);
            _p = _connectionHandler.parseMsg(serverMsg);
            processSuccessful = _p.msgCode == READY_TO_SEND_FILE;
            if (_p.msgCode == FILE_DOES_NOT_EXIST)
                cout << "File doesn't exist\nPlease try again" << endl;
        }
        catch (const exception& e)
        {
            cout << e.what();
        }
    } while (!processSuccessful);

    do
    {
        cout << "Enter the path where the file will be saved: ";
        cin >> pathToSaveFile;
        f.open(pathToSaveFile);
        processSuccessful = f.good();
        if (!processSuccessful)
            cout << "Invalid file path\nPlease try again\n";
    } while (!processSuccessful);

    while (_p.msgCode != FINISHED_SENDING_FILE && processSuccessful)
    {
        _connectionHandler.receiveMessage(serverMsg);
        _p = _connectionHandler.parseMsg(serverMsg);
        processSuccessful = _p.msgCode == FILE_DATA;
        if(processSuccessful)
            fileContent += _p.data;
    }
    if(processSuccessful)
        f << fileContent;
    delete[] serverMsg;
}

const string& Client::extractFileName(const string& filePath)
{
    string fileName = "";
    for (auto i : filePath)
        fileName = (i == '/' || i == '\\') ? "" : fileName + i;
    return fileName;
}

string Client::buildMsg(int msgCode, const string& username, const string& password, const string& data)
{
    return to_string(msgCode) +
        formatLen(to_string(username.length()), 2) + username +
        formatLen(to_string(password.length()), 2) + password +
        formatLen(to_string(data.length()), 3) + data;
}

string Client::formatLen(const string& len, int bytes)
{
    string formatted = len;
    for (int i = 0; i < bytes - len.length(); i++)
        formatted = '0' + formatted;
    return formatted;
}