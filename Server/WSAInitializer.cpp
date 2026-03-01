#include "WSAInitializer.h"
#include <exception>

/*Initializes the Winsock library
input: none
output: none*/
WSAInitializer::WSAInitializer()
{
	WSADATA wsa_data = { };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) throw std::exception("WSAStartup Failed");
}

/*Cleans up the Winsock library
input: none
output: none*/
WSAInitializer::~WSAInitializer()
{
	try
	{
		WSACleanup();
	}
	catch (...) {}
}