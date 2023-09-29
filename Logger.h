#pragma once
#include <iostream>
#include <filesystem>
#include <shared_mutex>
#include <thread>

class Logger
{
private:
	std::shared_mutex smtx;
	std::thread th;

public:
	Logger();
	~Logger();

	std::string getLastLine();
	void writeLine(const std::string&);
};