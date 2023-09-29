#include "Logger.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <format>

namespace fs = std::filesystem;

Logger::Logger()
{

    std::ifstream log;
    smtx.lock();
    log.open("log.txt", std::ios::in);
    if (!log.is_open())
        std::ofstream outfile("log.txt"); // create file in case it's not there
    
    smtx.unlock();
}

Logger::~Logger()
{
    smtx.lock();
    fs::remove("log.txt");
    smtx.unlock();
}

std::string Logger::getLastLine()
{
    std::ifstream log;
    smtx.lock_shared();
    log.open("log.txt", std::ios::in);
    std::string message;
    while (!log.eof())
    {
        std::string record;

        /*         std::getline(log, record);
                {
                    std::istringstream ss(record);
                    std::string token;
                    std::getline(ss, token, '\n');
                    message = token;
                } */
    }
    smtx.unlock_shared();
    return message;
}

void Logger::writeLine(const std::string& logRecord)
{
    time_t const ltime = time(NULL);
    std::stringstream transTime;
    transTime << std::put_time(localtime(&ltime), "%a %d %b %Y - %I:%M:%S%p");
    std::string result = transTime.str() + " | " + logRecord;

    smtx.lock();
    
    std::ofstream out("log.txt", std::ios::app);
    if (out.is_open())
        out << result << std::endl;
    
    smtx.unlock();
}