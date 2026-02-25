#include "Message.h"

Message::Message(int messageCode)
{
	_messageCode = messageCode;
}

int Message::getMessageCode()
{
	return _messageCode;
}