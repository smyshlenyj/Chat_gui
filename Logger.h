#pragma once
#include <iostream>
#include <filesystem>
#include <shared_mutex>

class Logger
{
private:
	std::shared_mutex smtx;

public:
	Logger();
	~Logger();

	std::string getLastLine();
	void writeLine(const std::string&);
};