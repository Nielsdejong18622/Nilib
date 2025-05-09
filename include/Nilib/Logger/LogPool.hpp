#ifndef _LOGGERPOOL_H
#define _LOGGERPOOL_H

#include "Nilib/Logger/Loggers.hpp"

#include <iostream>
#include <unordered_map>

namespace Nilib
{

    // Singleton LoggerPool class.
    // Holds the Logger registry.
    class LoggerPool
    {
        std::unordered_map<char const *, Logger *> d_logger_registry;

    public:
        static LoggerPool &instance();
        static Logger *global();
        Logger *name(char const *loggername);

        void register_logger(char const *loggername, Logger *logger);
        bool exists_logger(char const *loggername) const;
        
        // Flush all loggers. 
        void flush_loggers() const;
    };

};

#endif