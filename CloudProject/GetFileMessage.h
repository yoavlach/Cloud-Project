#pragma once
#include "Message.h"
#include <string>

class GetFileMessage : protected Message
{
public:
	GetFileMessage(const std::string& fileName, const std::string& pathToSaveFile, int messageCode);
	const std::string& getFileName();
	const std::string& getPathToSaveFile();
private:
	std::string _fileName;
	std::string _pathToSaveFile;
};