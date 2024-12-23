#ifndef _LOGGERPOOL_H
#define _LOGGERPOOL_H

#include "Nilib/Core/Loggers.hpp"

#include <iostream>
#include <unordered_map>

namespace Nilib {

    // Singleton LoggerPool class.
    // Holds the Logger registry.
    class LoggerPool {
        std::unordered_map<char const *, Logger*> d_logger_registry;
    public:
        static LoggerPool &instance(); 
        static Logger *global();
        Logger* name(char const *loggername);

        void register_logger(char const *loggername, Logger *logger);
    };

};

#endif