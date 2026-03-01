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
	string msg = "", currFileContent = "", currLine = "";
	char* clientMsg = new char[MAX_CLIENT_MESSAGE_LEN];
	ifstream f(_p.data.c_str());
	bool processSuccessful = true;
	if (!f.good())
		throw exception("File doesn't exist");
	_connectionHandler.setSocket(_socket);
	_connectionHandler.sendMessage(msg.c_str());
	while (getline(f, currLine) && processSuccessful)
	{
		if (currFileContent.length() + currLine.length() >= 999)
		{
			msg = to_string(FILE_DATA) + ":" + to_string(currFileContent.length()) + ":" + currFileContent;
			currFileContent = "";
			try
			{
				_connectionHandler.sendMessage(msg.c_str());
				_connectionHandler.receiveMessage(clientMsg);
				_p = parseMsg(clientMsg);
				if (_p.msgCode != FILE_DATA_RECEIVED)
				{
					processSuccessful = false;
					msg = to_string(GENERAL_INVALID_MESSAGE);
					_connectionHandler.sendMessage(msg.c_str());
				}
			}
			catch (const exception& e)
			{
				processSuccessful = false;
				cout << e.what() << endl;
				msg = to_string(GENERAL_INVALID_MESSAGE);
				_connectionHandler.sendMessage(msg.c_str());
			}
		}
		currFileContent += currLine;
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
	delete clientMsg;
}

void MessageHandler::sendFile()
{
	string fileContent = "", msg = "";
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
			msg = to_string(GENERAL_INVALID_MESSAGE);
			_connectionHandler.sendMessage(msg.c_str());
		}
		fileContent += _p.data;
	}
	f << fileContent;
	delete clientMessage;
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

const string& MessageHandler::getMsgPart(int& iterator, string& buffer, const string& msg)
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
		_connectionHandler.sendMessage(to_string(GENERAL_INVALID_MESSAGE).c_str());
		break;
	}
}