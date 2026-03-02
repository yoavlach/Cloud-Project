#include <iostream>
#include "Client.h"
#include "WSAInitializer.h"

using namespace std;

enum OPTIONS { LOGIN, SIGNUP, SEND_FILE, RECEIVE_FILE, EXIT };
int main()
{	
	Client c;
	WSAInitializer wsai;
	int action = 0;
	do
	{
		cout << "Choose action:\n1) Login\n2)Sign up\nYour choice: ";
		cin >> action;
	} while (action != LOGIN && action != SIGNUP);
	switch (action)
	{
	case LOGIN:
		c.login();
		break;
	case SIGNUP:
		c.signup();
		break;
	}
	do
	{
		do
		{
			cout << "Choose action\n3) Send file\n4) Receive file\n5) Exit\nYour choice: ";
			cin >> action;
		} while (action > SEND_FILE || action > EXIT);
		switch (action)
		{
		case SEND_FILE:
			c.sendFile();
			break;
		case RECEIVE_FILE:
			c.receiveFile();
			break;
		}
	} while (action != EXIT);
	cout << "Goodbye!";
	return 0;
}