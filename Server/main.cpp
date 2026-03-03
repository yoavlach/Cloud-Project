#include <iostream>
#include "Server.h"

int main()
{
	try
	{
		Server myServer;
		myServer.waitForClient();
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}