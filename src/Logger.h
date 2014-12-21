#pragma once
#include "globals.h"

#ifdef ENABLE_LOGGER
 
#define LOGGER_START(MIN_PRIORITY, FILE) Logger::Start(MIN_PRIORITY, FILE);
#define LOGGER_STOP() Logger::Stop();
#define LOGGER_WRITE(PRIORITY, MESSAGE) Logger::Write(PRIORITY, MESSAGE);
#define LOGGER_CODE(CODE) CODE
 
#else
 
#define LOGGER_START(MIN_PRIORITY, FILE)
#define LOGGER_STOP()
#define LOGGER_WRITE(PRIORITY, MESSAGE)
#define LOGGER_CODE(CODE)
 
#endif

class Logger
{
public:
    // log priorities
    enum Priority
    {
        ALL,
        FPS,
        USERINPUT,
        DEBUG,
        CONFIG,
        INFO,
        GL_ERROR,
        WARNING,
        ERRORmsg,
        EMPTY,
        TABULATOR
    };
 
    // start/stop logging
    // - messages with priority >= minPriority will be written in log
    // - set logFile = "" to write to standard output
    static void Start(Priority minPriority, const char* logFile);
    static void Stop();
 
    // write message
    static void Write(Priority priority, const POINT& d);
    static void Write(Priority priority, const DWORD d);
    static void Write(Priority priority, const double);
	static void Write(Priority priority, const std::string message);
    static void Write(Priority priority, const char* message);
 
private:
    // Logger adheres to the singleton design pattern, hence the private
    // constructor, copy constructor and assignment operator.
    Logger();
    Logger(const Logger& logger);
 
    // private instance data
    bool        active;
    Priority    minPriority;
 
    // names describing the items in enum Priority
    static const std::string PRIORITY_NAMES[];
    // the sole Logger instance (singleton)
    static Logger instance;
};
