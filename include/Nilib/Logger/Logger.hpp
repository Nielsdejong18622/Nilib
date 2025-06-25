#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include "Nilib/Logger/BaseLogger.h"

namespace Nilib
{

    enum class LogLevel
    {
        Info = 0,     // Default
        Success = 2,  // ON SUCCESS
        Warning = 14, // WARNING
        Error = 4,    // ERROR
        Debug = 8,    // DEBUG

        Trace = 8,   // Most detailed debug.
        Progress = 0 // To show to the end user.
    };

    // Can be derived from to support custom behaviour.
    class Logger : public BaseLogger
    {
    protected:
        LogLevel d_log;
        std::ostream &d_stream;

        virtual ~Logger() = default;
        virtual void insertTimestamp(LogLevel const level) = 0;
        virtual void insertColor(LogLevel const level) = 0;

        bool d_requireheader = true;

    public:
        bool endl = true;
        char sep = ' ';

        Logger(std::ostream &stream)
            : d_stream(stream)
        {
        }

        // This is called when the user does LOG_DEBUG(A, B, C, ...)
        Logger &userblock(LogLevel const level)
        {
            d_requireheader = true;
            insertColor(level);
            insertTimestamp(level);
            return *this;
        }

        // This is called when the user does LOG_DEBUG() <<
        Logger &userblock()
        {
            if (d_requireheader == true)
            {
                insertColor(d_log);
                insertTimestamp(d_log);
                d_requireheader = false;
            }
            return *this;
        }

        Logger &level(LogLevel const level)
        {
            d_log = level;
            // if (d_requireheader)
            // {
            //     insertColor(level);
            //     insertTimestamp(level);
            // }
            return *this;
        }

        Logger &output()
        {
            return *this;
        }

        // Recursive case: Print the first argument and then recurse with the rest
        template <typename T, typename... Args>
        void output(T &&first, Args &&...args)
        {
            d_stream << first; // Print the first argument
            if constexpr (sizeof...(args) > 0)
            {                                        // Check if there are more arguments
                d_stream << sep;                     // Print separator
                output(std::forward<Args>(args)...); // Template recurse
            }
            else if (endl)
            {
                d_stream << '\n';
                d_requireheader = true;
            }
            else
            {
                d_requireheader = true;
            }
        }

        void flush()
        {
            d_stream.flush();
        }

        template <typename T>
        constexpr Logger &operator<<(T const &message)
        {
            (d_stream) << message;
            return *this;
        }

        Logger &operator<<(char const &message)
        {
            if (message == '\n' || message == '\r')
            {
                d_requireheader = true;
            }
            (d_stream) << message;
            return *this;
        }
        
    };
};
#endif