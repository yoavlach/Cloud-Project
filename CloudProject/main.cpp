#include <iostream>
#include "UserAuth.h"

int main()
{
	UserAuth ua("Yov", "13");
	try
	{
		//ua.signup();
		ua.login();
		cout << "Successfully logged in" << endl;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}