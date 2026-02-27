#include <iostream>
#include "Server.h"
#include "Helper.h"

int main()
{
	Helper h;
	h.setMsg("10002hi03abc00");
	Packet p = h.pargeMsg();
	return 0;
}