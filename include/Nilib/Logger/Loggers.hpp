#ifndef _FILE_LOGGER_H
#define _FILE_LOGGER_H

#include "Nilib/Logger/Logger.hpp"
#include <chrono>
#include <fstream>

#ifdef _WIN32
#include "windows.h"
#endif

namespace Nilib
{

    // Log to file.
    class BasicFileLogger : public Logger
    {
        std::ofstream d_fstream;
        std::string d_filename;

    public:
        BasicFileLogger(std::string const &filename)
            : d_fstream(filename, std::ios::out), d_filename(filename), Logger(d_fstream)
        {
        }
        BasicFileLogger(char const *filename)
            : d_fstream(filename, std::ios::out), d_filename(filename), Logger(d_fstream)
        {
        }

        ~BasicFileLogger()
        {
            close();
        }

        void insertTimestamp(LogLevel const)
        {
        }

        void insertColor(LogLevel const)
        {
        }

        // Close the file (flush the buffer)
        void close()
        {
            d_fstream.flush();
            d_fstream.close();
        }

        void open()
        {
            d_fstream = std::ofstream(d_filename, std::ios::out);
        }

        // Close and open the File. 
        void reset()
        {
            close();
            open();
        }
    };

    // Log to console with timestamp and color (BASIC LOGGER).
    class BasicConsoleLogger : public Logger
    {
    public:
        BasicConsoleLogger(std::ostream &stream)
            : Logger(stream)
        {
        }

        ~BasicConsoleLogger()
        {
            // The reset color.
            insertColor(LogLevel::Info);
        }

        void insertTimestamp(LogLevel const)
        {
            // Print the formatted timestamp with microseconds
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();

            // Convert microseconds to a time_point with seconds precision
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
            auto time_point_seconds = std::chrono::system_clock::time_point(seconds);

            // Convert the time_point to a std::time_t
            std::time_t time_t_seconds = std::chrono::system_clock::to_time_t(time_point_seconds);

            // Convert the std::time_t to a tm structure for formatting
            std::tm *timeinfo = std::localtime(&time_t_seconds);

            // Format the timestamp as a string
            char buffer[20];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

            d_stream << "[" << buffer << "]:"; //<< "." << mics << pad;
        }

        void insertColor(LogLevel const level)
        {
// Set the text color based on the platform
#ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (level != LogLevel::Info)
                SetConsoleTextAttribute(hConsole, static_cast<WORD>(level));
            else
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

#ifdef __linux__
            switch (level)
            {
            case LogLevel::Error:
                (d_stream) << "\033[31m";
                return;
            case LogLevel::Warning:
                (d_stream) << "\033[33m";
                return;
            case LogLevel::Success:
                (d_stream) << "\033[32m";
                return;
            case LogLevel::Debug:
                (d_stream) << "\033[90m";
                return;
            default:
                (d_stream) << "\033[0m";
                return;
            }
#endif
        };
    };

}
#endif