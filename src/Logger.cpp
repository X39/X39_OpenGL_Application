#include "Logger.h"
#include <iostream>
#include <fstream>
 
using namespace std;
 
 
// --------------------------------------
// static members initialization
// --------------------------------------
 
const string Logger::PRIORITY_NAMES[] =
{
    "",
    "[FPS]\t",
    "[USERINPUT]",
    "[DEBUG]\t",
    "[CONFIG]\t",
    "[INFO]\t\t",
    "[WARNING]\t",
    "[ERROR]\t",
	"[GL_ERROR]\t",
    "",
    "\t\t\t"
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

void Logger::Write(Priority priority, const DWORD d)
{
	char str[64];
	sprintf(str, "%d", d);
	Write(priority, str);
}
void Logger::Write(Priority priority, const double d)
{
	char str[64];
	sprintf(str, "%lf", d);
	Write(priority, str);
}

void Logger::Write(Priority priority, const std::string message)
{
	Write(priority, message.c_str());
}
void Logger::Write(Priority priority, const char* message)
{
    if (instance.active && priority >= instance.minPriority)
    {
        // identify current output stream
        ostream& stream = ((fileStream.is_open()) ? (fileStream) : (std::cout));
 
        stream  << PRIORITY_NAMES[priority]
				<< ' '
                << message
                << endl;
#ifdef ENABLE_LOGGER_ToConsole
		cout << PRIORITY_NAMES[priority]
				<< ' '
                << message
                << endl;
#endif
    }
}