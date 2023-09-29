#pragma once
#include <fstream>
#include "Connection.h"
#include "Logger.h"
#include <thread>

struct Message
{
private:
	std::string sender, recipient, message;

public:
	Message(std::string const&, std::string const&, std::string const&);

	void sendMessage(int, Logger&); // push message to data base
};