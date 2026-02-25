#include "GetFileMessage.h"

GetFileMessage::GetFileMessage(const std::string& fileName, const std::string& pathToSaveFile, int messageCode)
	:Message(messageCode)
{
	_fileName = fileName;
	_pathToSaveFile = pathToSaveFile;
}

const std::string& GetFileMessage::getFileName()
{
	return _fileName;
}

const std::string& GetFileMessage::getPathToSaveFile()
{
	return _pathToSaveFile;
}