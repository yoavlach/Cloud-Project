#pragma once

class Message
{
public:
	Message(int messageCode);
	int getMessageCode();
private:
	int _messageCode;
};