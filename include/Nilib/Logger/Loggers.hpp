#ifndef _FILE_LOGGER_H
#define _FILE_LOGGER_H

#include "Nilib/Logger/Logger.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace Nilib
{

// Log to file.
class BasicFileLogger : public Logger
{
    std::ofstream d_fstream;
    std::filesystem::path d_filename;

  public:
    explicit BasicFileLogger(std::filesystem::path const &filename)
        : Logger(d_fstream), d_fstream(filename, std::ios::out), d_filename(filename)
    {
    }

    explicit BasicFileLogger(char const *filename)
        : Logger(d_fstream), d_fstream(filename, std::ios::out), d_filename(filename)
    {
    }

    ~BasicFileLogger() override
    {
        close();
    }

    void insertTimestamp(LogLevel const) override
    {
    }

    void insertColor(LogLevel const) override
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
    explicit BasicConsoleLogger(std::ostream &stream) : Logger(stream)
    {
    }

    ~BasicConsoleLogger() override
    {
        // The reset color.
        insertColor(LogLevel::Info);
    }

    void insertTimestamp(LogLevel const) override
    {
        // Print the formatted timestamp with microseconds
        auto now = std::chrono::system_clock::now();

        // Format as "YYYY-MM-DD HH:MM:SS"
        std::string formatted_time = std::format("{:%Y-%m-%d %H:%M:%S}", now);
        d_stream << "[" << formatted_time << "]:"; 
    }

    void insertColor(LogLevel const level) override
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

} // namespace Nilib
#endif