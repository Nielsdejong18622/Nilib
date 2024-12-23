#include "Nilib/Logger/LogPool.hpp"

using namespace Nilib;

// Get the one and only singleton.
LoggerPool &LoggerPool::instance() {
    static LoggerPool logpool;
    return logpool;
}

// Returns the default global logger. 
Logger *LoggerPool::global() {
    static BasicConsoleLogger g_logger(std::cout);
    return &g_logger;
}

// Return the logger from the logpool.
// If loggername is not registered, print error and return global logger.
Logger* LoggerPool::name(char const *loggername)
{
    if (d_logger_registry.contains(loggername))
        return d_logger_registry[loggername];
    
    //TODO fail using global logger!
    LoggerPool::global()->level(LogLevel::Warning) << "Logger: " << loggername << " not registered!\n";
    return LoggerPool::global(); // No logger with that name was found. 
}

void LoggerPool::register_logger(char const *loggername, Logger *logger)
{
    d_logger_registry.insert_or_assign(loggername, logger);
}