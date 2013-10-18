#include "Logger.h"
#include <iostream>
#include <fstream>
 
using namespace std;
 
 
// --------------------------------------
// static members initialization
// --------------------------------------
 
const string Logger::PRIORITY_NAMES[] =
{
    "DEBUG",
    "CONFIG",
    "INFO",
    "WARNING",
    "ERROR"
};
 
Logger Logger::instance;
static std::ofstream fileStream;
 
 
// --------------------------------------
// function implementations
// --------------------------------------
 
Logger::Logger() : active(false) {}

Logger::Logger(const Logger& logger){}
 
void Logger::Start(Priority minPriority, const char* logFile)
{
    instance.active = true;
    instance.minPriority = minPriority;
    if (logFile != "")
    {
        fileStream.open(logFile);
    }
}
 
void Logger::Stop()
{
    instance.active = false;
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}
 
void Logger::Write(Priority priority, const char* message)
{
    if (instance.active && priority >= instance.minPriority)
    {
        // identify current output stream
        ostream& stream = ((fileStream.is_open()) ? (fileStream) : (std::cout));
 
        stream  << PRIORITY_NAMES[priority]
                << ": "
                << message
                << endl;
#ifdef ENABLE_LOGGER_ToConsole
		cout << PRIORITY_NAMES[priority]
                << ": "
                << message
                << endl;
#endif
    }
}