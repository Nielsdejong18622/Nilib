#ifndef _CSV_HPP
#define _CSV_HPP

#include "Nilib/Logger/Log.hpp"
#include <sstream>

namespace Nilib::CSVhelper
{
    template <typename T>
    void stream_csv_line(std::ostringstream &oss, T &&value)
    {
        oss << std::forward<T>(value);
    }

    template <typename T, typename... Rest>
    void stream_csv_line(std::ostringstream &oss, T &&first, Rest &&...rest)
    {
        oss << std::forward<T>(first) << ',';
        stream_csv_line(oss, std::forward<Rest>(rest)...);
    }

    template <typename... Args>
    std::string to_csv_line(Args &&...args)
    {
        std::ostringstream oss;
        stream_csv_line(oss, std::forward<Args>(args)...);
        return oss.str();
    }
}

// MACRO for dumping variables to CSVfile. Automatically inserts header based on variable name.
#define CSV_STRINGIFY(...) #__VA_ARGS__
#define CSV(filename, ...)                                                                    \
    do                                                                                        \
    {                                                                                         \
        if (!Nilib::LoggerPool::instance().exists_logger(filename))                           \
        {                                                                                     \
            static Nilib::BasicFileLogger s_##filename_csvlogger(filename);                   \
            REGISTER_LOGGER(filename, &s_##filename_csvlogger);                               \
            Nilib::LoggerPool::instance().name(filename)->sep = ',';                          \
            Nilib::LoggerPool::instance().name(filename)->output(CSV_STRINGIFY(__VA_ARGS__)); \
        }                                                                                     \
        Nilib::LoggerPool::instance().name(filename)->output(__VA_ARGS__);                    \
    } while (0)

#endif