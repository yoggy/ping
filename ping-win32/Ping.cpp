#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Ping.h"
#include <Ws2tcpip.h>

class PingInitializer {
public:
	PingInitializer() {
		WSADATA data;
		WSAStartup(MAKEWORD(2, 0), &data);
	}
};

PingInitializer singleton_;

Ping::Ping() : host_(), timeout_(200)
{
	handle_ = ::IcmpCreateFile();
	::UuidCreate(&uuid_);
}

Ping::Ping(const std::string &host) : host_(host), timeout_(200)
{
	handle_ = ::IcmpCreateFile();
	::UuidCreate(&uuid_);
}

Ping::~Ping()
{
	::IcmpCloseHandle(handle_);
}

// noncopyable
Ping::Ping(const Ping &p)
{
}

// noncopyable
Ping & Ping::operator=(const Ping &p)
{
	return *this;
}

std::string Ping::host() const
{
	return host_;
}

void Ping::host(const std::string &val)
{
	host_ = val;
}

int Ping::timeout() const
{
	return timeout_;
}

void Ping::timeout(const int &val)
{
	timeout_ = val;
}

bool Ping::is_alive()
{
	DWORD rv;

	// hostname -> IPAddr
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	getaddrinfo(host_.c_str(), NULL, &hints, &res);

	IPAddr ip_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.S_un.S_addr;

	// build send packet payload
	char send_buf[sizeof(UUID) + sizeof(DWORD)];
	memset(send_buf, 0, sizeof(send_buf));
	memcpy(send_buf, (void*)&uuid_, sizeof(UUID));

	DWORD t = ::GetTickCount();
	memcpy(send_buf + sizeof(UUID), (void*)&t, sizeof(DWORD));

	// receive buffer
	char recv_buf[sizeof(ICMP_ECHO_REPLY) + sizeof(send_buf)];
	memset(recv_buf, 0, sizeof(recv_buf));

	rv = ::IcmpSendEcho(
		handle_,
		ip_addr,
		send_buf,
		sizeof(send_buf),
		NULL,
		recv_buf,
		sizeof(recv_buf), 
		timeout_
		);

	if (rv == 0) {
		LPVOID msg;

		DWORD err = ::GetLastError();
		ERROR_INSUFFICIENT_BUFFER;
		ERROR_INVALID_PARAMETER;

		ERROR_NOT_ENOUGH_MEMORY;
		IP_BUF_TOO_SMALL;

		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&msg,
			0,
			NULL);

		printf("Ping::is_alive() : error...%s\n", msg);

		::LocalFree(msg);

		return false;
	}

	return true;
}
