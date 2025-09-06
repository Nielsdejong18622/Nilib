#ifndef _CSV_HPP
#define _CSV_HPP

#include "Nilib/Logger/Log.hpp"
#include <sstream>

// MACRO for dumping variables to CSVfile. Automatically inserts header based on variable names.
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