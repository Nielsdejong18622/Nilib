#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include "Nilib/Core/BaseLogger.h"

namespace Nilib {

    enum class LogLevel {
        Info = 0, // Default
        Success = 2, // ON SUCCESS
        Warning = 14, // WARNING
        Error = 4, // ERROR
        Debug = 8,//DEBUG
        
        Trace = 8, // Most detailed debug. 
        Progress = 0 // To show to the end user. 
    };

    // Can be derived from to support custom behaviour. 
    class Logger : public BaseLogger {
    protected:
        LogLevel d_log;
        std::ostream &d_stream;
    public:

        Logger(std::ostream &stream)
            : d_stream(stream)
        {
        }

        virtual ~Logger() = default;
        virtual void insertTimestamp(LogLevel const level) = 0;
        virtual void insertColor(LogLevel const level) = 0;

        Logger &level(LogLevel const level)
        {
            insertColor(level);
            insertTimestamp(level);
            return *this;
        }

        Logger &output() {
            return *this;
        }

        // Recursive case: Print the first argument and then recurse with the rest
        template <typename T, typename... Args>
        void output(T&& first, Args&&... args) {
            d_stream << first;  // Print the first argument
            if constexpr (sizeof...(args) > 0) {  // Check if there are more arguments
                d_stream << ' ';  // Print separator
                output(std::forward<Args>(args)...);  // Template recurse
            } else {
                d_stream << '\n';
            }
        }

        template<typename T>    
        Logger &operator<<(T const &message) {
            (d_stream) << message;
            return *this;
        }
    };
};
#endif