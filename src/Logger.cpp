#include "globals.h"
#include <iostream>
#include <fstream>

#include "Logger.h"
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
#ifdef ENABLE_LOGGER_ToFile
    if (logFile != "")
    {
        fileStream.open(logFile);
    }
#endif
}
 
void Logger::Stop()
{
    instance.active = false;
#ifdef ENABLE_LOGGER_ToFile
    if (fileStream.is_open())
    {
        fileStream.close();
    }
#endif
}
void Logger::Write(Priority priority, const POINT& d)
{
	char str[64];
	sprintf(str, "x: %d | y: %d", d.x, d.y);
	Write(priority, str);
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
#ifdef ENABLE_LOGGER_ToFile
    if (instance.active && priority >= instance.minPriority)
    {
        // identify current output stream
        ostream& stream = ((fileStream.is_open()) ? (fileStream) : (std::cout));
 
        stream  << PRIORITY_NAMES[priority]
				<< ' '
                << message
                << endl;
    }
#endif
#ifdef ENABLE_LOGGER_ToConsole
	cout << PRIORITY_NAMES[priority]
			<< ' '
            << message
            << endl;
#endif
}