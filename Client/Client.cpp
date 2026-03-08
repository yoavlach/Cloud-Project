#include "Client.h"
enum RECEIVE_FILES_OPTIONS { CREATE_NEW_FILE = 1, OVERWRITE_EXISTING};
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
    string username = "", password = "";
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
            _connectionHandler.sendMessage(buildMsg(LOGIN, username, password, "").c_str());
            chMsg = new char[MAX_SERVER_MESSAGE_LEN];
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
    string username = "", password = "";
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
            chMsg = new char[MAX_SERVER_MESSAGE_LEN];
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
    string path = "", currLine = "", currFileContent = "", fileName = "";
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
        _connectionHandler.receiveMessage(serverMsg);
        _p = _connectionHandler.parseMsg(serverMsg);
        if (_p.msgCode != READY_TO_RECEIVE_FILE)
            cout << "Error\nPlese try again" << endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }
    while (getline(f, currLine) && processSuccessful)
    {
        currLine += "\n";
        if (currLine.length() <= 999)
        {
            try
            {
                _connectionHandler.sendMessage(buildMsg(FILE_DATA, "", "", currLine).c_str());
                _connectionHandler.receiveMessage(serverMsg);
                _p = _connectionHandler.parseMsg(serverMsg);
                processSuccessful = _p.msgCode == FILE_DATA_RECEIVED;
                if (!processSuccessful)
                    cerr << "Error occured" << endl;
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
            _connectionHandler.sendMessage(buildMsg(FINISHED_SENDING_FILE, "", "", "").c_str());
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
    int action = 0;
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
        cout << "1) Create a new file\n2) Overwrite an existing file\nEnter your choice: ";
        cin >> action;
    } while (action != OVERWRITE_EXISTING && action != CREATE_NEW_FILE);
    switch (action)
    {
    case OVERWRITE_EXISTING:
        do
        {
            cout << "Enter the path where the file will be saved: ";
            cin >> pathToSaveFile;
            f.open(pathToSaveFile);
            processSuccessful = f.good();
            if (!processSuccessful)
                cout << "Invalid file path\nPlease try again\n";
        } while (!processSuccessful);
        break;
    case CREATE_NEW_FILE:
        do
        {
            cout << "Enter the path for the new file: ";
            cin >> pathToSaveFile;
            f.open(pathToSaveFile + "\\" + fileName);
            processSuccessful = f.good();
            if (!processSuccessful)
                cout << "Invalid file path\nPlease try again\n";
        } while (!processSuccessful);
    }

    while (_p.msgCode != FINISHED_SENDING_FILE && processSuccessful)
    {
        _connectionHandler.receiveMessage(serverMsg);
        _p = _connectionHandler.parseMsg(serverMsg);
        if (_p.msgCode == FILE_DATA)
        {
            _connectionHandler.sendMessage(buildMsg(FILE_DATA_RECEIVED, "", "", "").c_str());
            fileContent += _p.data;
        }
        else if (_p.msgCode != FINISHED_SENDING_FILE)
            processSuccessful = false;
    }
    if(processSuccessful)
        f << fileContent;
    delete[] serverMsg;
    f.close();
}

void Client::getListOfFiles()
{
    string listOfFiles = "";
    char* serverMsg = new char[MAX_SERVER_MESSAGE_LEN];
    char saveFilesListInFile = ' ';
    bool processSuccessful = true;
    _connectionHandler.sendMessage(buildMsg(GET_LIST_OF_FILES, "", "", "").c_str());
    _connectionHandler.receiveMessage(serverMsg);
    _p = _connectionHandler.parseMsg(serverMsg);
    if (_p.msgCode == FILE_LIST_EMPTY)
        cout << "You have no files yet" << endl;
    else if (_p.msgCode == READY_TO_SEND_FILE)
    {
        while (_p.msgCode != FINISHED_SENDING_FILE && processSuccessful)
        {
            _connectionHandler.receiveMessage(serverMsg);
            _p = _connectionHandler.parseMsg(serverMsg);
            if (_p.msgCode == FILE_DATA)
            {
                _connectionHandler.sendMessage(buildMsg(FILE_DATA_RECEIVED, "", "", "").c_str());
                listOfFiles += _p.data;
            }
            else if (_p.msgCode != FINISHED_SENDING_FILE)
                processSuccessful = false;
        }
        if (processSuccessful)
        {
            processSuccessful = false;
            cout << "Your files:\n" << listOfFiles << endl;
        }
    }
}

string Client::extractFileName(const string& filePath)
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