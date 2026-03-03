#include "MessageHandler.h"

MessageHandler::MessageHandler()
{
	_usersManager;
	_p;
}

void MessageHandler::setPacket(const Packet& p)
{
	_p = p;
}

void MessageHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
}

void MessageHandler::login()
{
	string passwordBuffer = "";
	if (!_usersManager.searchUsername(_p.username)) throw exception("Username does not exist");
	_usersManager.getPassword(_p.username, passwordBuffer);
	if (passwordBuffer != _p.password) throw exception("Incorrect password");
}
void MessageHandler::signup()
{
	if (_usersManager.searchUsername(_p.username)) throw exception("Username already exists");
	_usersManager.addUser(_p.username, _p.password);
}

void MessageHandler::getFile()
{
	string currFileContent = "", currLine = "";
	char* clientMsg = new char[MAX_CLIENT_MESSAGE_LEN];
	ifstream f(_p.data.c_str());
	bool processSuccessful = true;
	if (!f.good())
	{
		_connectionHandler.sendMessage(buildMsg(FILE_DOES_NOT_EXIST, "").c_str());
		throw exception("File doesn't exist");
	}
	_connectionHandler.setSocket(_socket);
	_connectionHandler.sendMessage(buildMsg(READY_TO_SEND_FILE, "").c_str());
	while (getline(f, currLine) && processSuccessful)
	{
		if (currFileContent.length() + currLine.length() >= 999)
		{
			currFileContent = "";
			try
			{
				_connectionHandler.sendMessage(buildMsg(FILE_DATA, currFileContent).c_str());
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
		currFileContent += currLine;
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
	string fileContent = "";
	char* clientMessage = new char[MAX_CLIENT_MESSAGE_LEN];
	bool processSuccessful = true;
	ofstream f(_p.data.c_str());
	while (_p.msgCode != FINISHED_SENDING_FILE && processSuccessful)
	{
		_connectionHandler.receiveMessage(clientMessage);
		_p = parseMsg(clientMessage);
		if (_p.msgCode != FILE_DATA)
		{
			processSuccessful = false;
			_connectionHandler.sendMessage(buildMsg(GENERAL_INVALID_MESSAGE, "").c_str());
		}
		fileContent += _p.data;
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
	{
		msgCode += msg[i];
	}
	p->msgCode = stoi(msgCode);
	getMsgPart(i, p->username, msg);
	getMsgPart(i, p->password, msg);
	getMsgPart(i, p->data, msg);
	return *p;
}

string MessageHandler::getMsgPart(int& iterator, string& buffer, const string& msg)
{
	string strPartLen = "";
	int starterItVal = iterator, partLen = 0;
	for (; iterator < starterItVal + USERNAME_AND_PASSWORD_LEN_SIZE; iterator++)
	{
		strPartLen += msg[iterator];
	}
	partLen = stoi(strPartLen);
	starterItVal = iterator;
	for (; iterator < starterItVal + partLen; iterator++)
	{
		buffer += msg[iterator];
	}
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
	string formatted = "";
	for (int i = 0; i < bytes - len.length(); i++)
		formatted = '0' + formatted;
	return formatted;
}