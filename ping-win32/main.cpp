#include <iostream>
#include "ping.h"

int main(int argc, char* argv[])
{
	std::string host = "192.168.1.1";

	Ping ping(host);

	while (true) {
		std::cout << "ping : host=" << host << ", is_alive=" << (ping.is_alive() ? "true" : "false") << std::endl;
		::Sleep(100);
	}

	return 0;
}

