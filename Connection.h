#pragma once
#include <iostream>
#include <string.h>
#include <sstream>
#include <string>
#include <vector>
#include "Users.h"
#include "Constants.h"
#include <list>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#pragma comment(lib, "ws2_32.lib") //
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class Connection
{
public:
	Connection();
	~Connection();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	SOCKET connectClientOpen();
#else
	int connectClientOpen();
#endif

	void connectClose(int socket_discriptor);

};