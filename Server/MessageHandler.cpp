#include "MessageHandler.h"
#include <Windows.h>

void MessageHandler::setPacket(const Packet& p)
{
	_p = p;
}

void MessageHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
	_connectionHandler.setSocket(socket);
}

void MessageHandler::login()
{
	string passwordBuffer = "";
	if (!_usersManager.searchUsername(_p.username))
	{
		_connectionHandler.sendMessage(buildMsg(USERNAME_DOESNT_EXIST, "").c_str());
		throw exception("Username does not exist");
	}
	_usersManager.getPassword(_p.username, passwordBuffer);
	if (passwordBuffer != _p.password)
	{
		_connectionHandler.sendMessage(buildMsg(INCORRECT_PASSWORD, "").c_str());
		throw exception("Incorrect password");
	}
	_connectionHandler.sendMessage(buildMsg(LOGIN_SUCCESSFUL, "").c_str());
	setConnectedUsername(_p.username);
}
void MessageHandler::signup()
{
	string dirPath = "usersData\\" + _p.username;
	if (_usersManager.searchUsername(_p.username))
	{
		_connectionHandler.sendMessage(buildMsg(USERNAME_ALREADY_EXISTS, "").c_str());
		throw exception("Username already exists");
	}
	_usersManager.addUser(_p.username, _p.password);
	_connectionHandler.sendMessage(buildMsg(SIGN_UP_SUCCESSFUL, "").c_str());
	CreateDirectoryA(dirPath.c_str(), NULL);
	setConnectedUsername(_p.username);
}

void MessageHandler::getFile()
{
	string currLine = "", filePath = "usersData\\" + _connectedUsername + "\\" + _p.data;
	char* clientMsg = new char[MAX_CLIENT_MESSAGE_LEN];
	bool processSuccessful = true;
	ifstream f(filePath.c_str());
	if (!f.good())
	{
		_connectionHandler.sendMessage(buildMsg(FILE_DOES_NOT_EXIST, "").c_str());
		throw exception("File doesn't exist");
	}
	_connectionHandler.setSocket(_socket);
	_connectionHandler.sendMessage(buildMsg(READY_TO_SEND_FILE, "").c_str());
	while (getline(f, currLine) && processSuccessful)
	{
		currLine += "\n";
		if (currLine.length() <= 999)
		{
			try
			{
				_connectionHandler.sendMessage(buildMsg(FILE_DATA, currLine).c_str());
				_connectionHandler.receiveMessage(clientMsg);
				_p = parseMsg(clientMsg);
				if (_p.msgCode != FILE_DATA_RECEIVED)
				{
					processSuccessful = false;
					_connectionHandler.sendMessage(buildMsg(GENERAL_INVALID_MESSAGE, "").c_str());
				}
			}
			catch (const exception& e)
			{
				processSuccessful = false;
				cout << e.what() << endl;
				_connectionHandler.sendMessage(buildMsg(GENERAL_INVALID_MESSAGE, "").c_str());
			}
		}
	}
	if (processSuccessful)
	{
		try
		{
			_connectionHandler.sendMessage(buildMsg(FINISHED_SENDING_FILE, "").c_str());
		}
		catch (const exception& e)
		{
			cout << e.what();
		}
	}
	f.close();
	delete[] clientMsg;
}

void MessageHandler::sendFile()
{
	string fileContent = "", filePath = "usersData\\" + _connectedUsername + "\\" + _p.data;
	char* clientMessage = new char[MAX_CLIENT_MESSAGE_LEN];
	bool processSuccessful = true;
	_connectionHandler.sendMessage(buildMsg(READY_TO_RECEIVE_FILE, "").c_str());
	ofstream f(filePath.c_str());
	while (_p.msgCode != FINISHED_SENDING_FILE && processSuccessful)
	{
		_connectionHandler.receiveMessage(clientMessage);
		_p = parseMsg(clientMessage);
		if(_p.msgCode == FILE_DATA)
		{
			_connectionHandler.sendMessage(buildMsg(FILE_DATA_RECEIVED, "").c_str());
			fileContent += _p.data;
		}
		else if (_p.msgCode != FINISHED_SENDING_FILE)
		{
			processSuccessful = false;
			_connectionHandler.sendMessage(buildMsg(GENERAL_INVALID_MESSAGE, "").c_str());
		}
	}
	f << fileContent;
	delete[] clientMessage;
}

Packet& MessageHandler::parseMsg(const string& msg)
{
	string msgCode = "";
	Packet* p = new Packet;
	int i = 0;
	for (i = 0; i < MSG_CODE_LEN; i++)
		msgCode += msg[i];
	p->msgCode = stoi(msgCode);
	getMsgPart(i, p->username, msg, USERNAME_AND_PASSWORD_LEN_SIZE);
	getMsgPart(i, p->password, msg, USERNAME_AND_PASSWORD_LEN_SIZE);
	getMsgPart(i, p->data, msg, DATA_LEN_SIZE);
	return *p;
}

string MessageHandler::getMsgPart(int& iterator, string& buffer, const string& msg, int lenSize)
{
	string strPartLen = "";
	int starterItVal = iterator, partLen = 0;
	for (; iterator < starterItVal + lenSize; iterator++)
		strPartLen += msg[iterator];
	partLen = stoi(strPartLen);
	starterItVal = iterator;
	for (; iterator < starterItVal + partLen; iterator++)
		buffer += msg[iterator];
	return buffer;
}

void MessageHandler::callMsgProcessFunc(const string& msg)
{
	_p = parseMsg(msg);
	switch (_p.msgCode)
	{
	case LOGIN:
		login();
		break;
	case SIGNUP:
		signup();
		break;
	case READY_TO_SEND_FILE:
		sendFile();
		break;
	case READY_TO_RECEIVE_FILE:
		getFile();
		break;
	default:
		_connectionHandler.sendMessage(buildMsg(GENERAL_INVALID_MESSAGE, "").c_str());
		break;
	}
}

string MessageHandler::buildMsg(int msgCode, const string& data)
{
	return to_string(msgCode) + formatLen(to_string(data.length()), 3) + data;
}

string MessageHandler::formatLen(const string& len, int bytes)
{
	string formatted = len;
	for (int i = 0; i < bytes - len.length(); i++)
		formatted = '0' + formatted;
	return formatted;
}


void MessageHandler::setConnectedUsername(const string& connectedUsername)
{
	_connectedUsername = connectedUsername;
}