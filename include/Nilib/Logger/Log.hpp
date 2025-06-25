#ifndef _LOGGING2_H
#define _LOGGING2_H

#include "Nilib/Logger/LogPool.hpp"

namespace Nilib
{

// Macro's
// Only LOG_LEVEL_TYPES higher than the current LOG_LEVEL are printed.
#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#define REGISTER_LOGGER(loggername, LOGGER_PTR) ::Nilib::LoggerPool::instance().register_logger(loggername, LOGGER_PTR)

// Standard Log levels.
#ifndef LOG_LEVEL_TRACE
#define LOG_LEVEL_TRACE 1
#endif
#ifndef LOG_LEVEL_DEBUG
#define LOG_LEVEL_DEBUG 2
#endif
#ifndef LOG_LEVEL_INFO
#define LOG_LEVEL_INFO 3
#endif
#ifndef LOG_LEVEL_WARNING
#define LOG_LEVEL_WARNING 4
#endif
#ifndef LOG_LEVEL_SUCCESS
#define LOG_LEVEL_SUCCESS 5
#endif
#ifndef LOG_LEVEL_ERROR
#define LOG_LEVEL_ERROR 5
#endif
#ifndef LOG_LEVEL_PROGRESS
#define LOG_LEVEL_PROGRESS 6
#endif

#if (LOG_LEVEL_TRACE > LOG_LEVEL)
#define LOG_TRACE_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Trace).output(__VA_OPT__(__VA_ARGS__))
#define LOG_TRACE(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Trace).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_TRACE_TO(logger, ...) Nilib::BaseLogger()
#define LOG_TRACE(...) Nilib::BaseLogger()
#endif

#if (LOG_LEVEL_DEBUG > LOG_LEVEL)
#define LOG_DEBUG_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Debug).userblock(__VA_OPT__(Nilib::LogLevel::Debug)).output(__VA_OPT__(__VA_ARGS__))
#define LOG_DEBUG(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Debug).userblock(__VA_OPT__(Nilib::LogLevel::Debug)).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_DEBUG_TO(logger, ...) Nilib::BaseLogger()
#define LOG_DEBUG(...) Nilib::BaseLogger()
#endif

#if (LOG_LEVEL_INFO > LOG_LEVEL)
#define LOG_INFO_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Info).userblock(__VA_OPT__(Nilib::LogLevel::Info)).output(__VA_OPT__(__VA_ARGS__))
#define LOG_INFO(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Info).userblock(__VA_OPT__(Nilib::LogLevel::Info)).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_INFO_TO(logger, ...) Nilib::BaseLogger()
#define LOG_INFO(...) Nilib::BaseLogger()
#endif

#if (LOG_LEVEL_WARNING > LOG_LEVEL)
#define LOG_WARNING_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Warning).userblock(__VA_OPT__(Nilib::LogLevel::Warning)).output(__VA_OPT__(__VA_ARGS__))
#define LOG_WARNING(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Warning).userblock(__VA_OPT__(Nilib::LogLevel::Warning)).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_WARNING_TO(logger, ...) Nilib::BaseLogger()
#define LOG_WARNING(...) Nilib::BaseLogger()
#endif

#if (LOG_LEVEL_SUCCESS > LOG_LEVEL)
#define LOG_SUCCESS_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Success).userblock(__VA_OPT__(Nilib::LogLevel::Success)).output(__VA_OPT__(__VA_ARGS__))
#define LOG_SUCCESS(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Success).userblock(__VA_OPT__(Nilib::LogLevel::Success)).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_SUCCESS_TO(logger, ...) Nilib::BaseLogger()
#define LOG_SUCCESS(...) Nilib::BaseLogger()
#endif

#if (LOG_LEVEL_ERROR > LOG_LEVEL)
#define LOG_ERROR_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Error).userblock(__VA_OPT__(Nilib::LogLevel::Error)).output(__VA_OPT__(__VA_ARGS__))
#define LOG_ERROR(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Error).userblock(__VA_OPT__(Nilib::LogLevel::Error)).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_ERROR_TO(logger, ...) Nilib::BaseLogger()
#define LOG_ERROR(...) Nilib::BaseLogger()
#endif

#if (LOG_LEVEL_PROGRESS > LOG_LEVEL)
#define LOG_PROGRESS_TO(logger, ...) Nilib::LoggerPool::instance().name(logger)->level(::Nilib::LogLevel::Progress).userblock(__VA_OPT__(Nilib::LogLevel::Progress)).output(__VA_OPT__(__VA_ARGS__))
#define LOG_PROGRESS(...) Nilib::LoggerPool::instance().global()->level(Nilib::LogLevel::Progress).userblock(__VA_OPT__(Nilib::LogLevel::Progress)).output(__VA_OPT__(__VA_ARGS__))
#else
#define LOG_PROGRESS_TO(logger, ...) Nilib::BaseLogger()
#define LOG_PROGRESS(...) Nilib::BaseLogger()
#endif

};

#endif