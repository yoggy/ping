#pragma once

#include <string>

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")

class Ping
{
public:
	Ping();
	Ping(const std::string &host);

	virtual ~Ping();

	std::string host() const;
	void host(const std::string &val);
	int timeout() const;
	void timeout(const int &val);

	bool is_alive();

private:
	// noncopyable
	Ping(const Ping &p);
	Ping & operator=(const Ping &p);

protected:
	std::string host_;
	int timeout_;
	HANDLE handle_;	

	UUID uuid_;
	
};

