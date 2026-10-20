#ifndef NILIB_LOGGING_H
#define NILIB_LOGGING_H

#include "Nilib/Logger/Format.hpp"

#include <chrono>
#include <cstdio>
#include <concepts>
#include <ctime>
#include <format>
#include <mutex>
#include <print>
#include <string>
#include <iostream>
#include <type_traits>
#include <utility>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Nilib
{
    // Severity and filtering order, not console color codes.
    enum class LogLevel : int
    {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Success = 3,
        Progress = 4,
        Warning = 5,
        Error = 6
    };
    enum class ColorMode
    {
        Auto,   // Color only when output is a supported terminal
        Always, // Always emit ANSI color codes
        Never   // Never emit color codes
    };

    class Logger final
    {
    public:
        Logger() = delete;

        template <class... Args>
        static void log(
            LogLevel level,
            bool const newline,
            std::type_identity_t<CheckedFormatString<Args...>> fmt,
            Args &&...args)
        {
            std::lock_guard<std::mutex> const lock(mutex());

            std::ostream &stream = *file(level);

            bool const use_color = colors_enabled(stream);
            char const *color = use_color ? color_code(level) : "";
            char const *reset = use_color ? "\033[0m" : "";
            char const term = newline ? '\n' : '\r';

            std::print(
                stream,
                "[{}|{}]:{}",
                timestamp(),
                std::this_thread::get_id(),
                color);

            std::print(stream, fmt.get(), std::forward<Args>(args)...);

            std::print(stream, "{}{}", reset, term);
        }

        // Overload of single string_view.
        // static void log(
        //     LogLevel level,
        //     bool const newline,
        //     std::string_view const &string)
        // {
        //     log(level, newline, std::format("{}", string));
        // }
        // static void log(
        //     LogLevel level,
        //     bool const newline,
        //     std::string const &string)
        // {
        //     log(level, newline, std::format("{}", string));
        // }
        // // Overload of custom type.
        // template <typename T>
        // static void log(
        //     LogLevel level,
        //     bool const newline,
        //     T const &type)
        // {
        //     log(level, newline, "{}", type);
        // // }

        // // Overload of char const *.
        // static void log(
        //     LogLevel level,
        //     bool const newline,
        //     char const *string)
        // {
        //     log(level, newline, std::format("{}", string));
        // }

        // // Overload for exception.
        // static void log(
        //     LogLevel level,
        //     bool const newline,
        //     std::exception const &exception)
        // {
        //     log(level, newline, std::format("{} {}", typeid(exception).name(), exception.what()));
        // }

        static void set_color_mode(ColorMode mode)
        {
            color_mode() = mode;
        }

        static void set_stream(std::ostream &stream)
        {
            Logger::file(LogLevel::Warning) = &stream;
            Logger::file(LogLevel::Info) = &stream;
        }

        static void sync_with_c(bool set = false)
        {
            std::ios::sync_with_stdio(set);
        }

    private:
        static std::mutex &mutex()
        {
            static std::mutex m;
            return m;
        }

        static std::ostream *&file(LogLevel level)
        {
            static std::ostream *out = &std::cout;
            static std::ostream *err = &std::cerr;

            if (level == LogLevel::Warning)
                return err;

            return out;
        }

        static ColorMode &color_mode()
        {
            static ColorMode mode = ColorMode::Auto;
            return mode;
        }

        static const char *color_code(LogLevel level)
        {
            switch (level)
            {
            case LogLevel::Error:
                return "\033[31m";
            case LogLevel::Warning:
                return "\033[33m";
            case LogLevel::Success:
                return "\033[32m";
            case LogLevel::Debug:
                return "\033[90m";
            default:
                return "\033[0m";
            }

            // Colors left over.
            // return "\033[36m"; // Cyan
            // return "\033[37m"; // White

            return "\033[0m"; // Reset.
        }

        static bool colors_enabled(std::ostream &stream)
        {
            const auto mode = color_mode();

            if (mode == ColorMode::Never)
                return false;

            if (mode == ColorMode::Always)
                return true;

#ifdef _WIN32
            DWORD id;

            if (&stream == &std::cerr || &stream == &std::clog)
                id = STD_ERROR_HANDLE;
            else if (&stream == &std::cout)
                id = STD_OUTPUT_HANDLE;
            else
                return false; // Custom streams, such as file streams.

            HANDLE handle = GetStdHandle(id);

            if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
                return false;

            DWORD console_mode = 0;

            if (!GetConsoleMode(handle, &console_mode))
                return false;

            if (!(console_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
            {
                if (!SetConsoleMode(
                        handle,
                        console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
                    return false;
            }

            return true;
#else
            int fd = -1;

            if (&stream == &std::cout || &stream == &std::clog)
                fd = STDOUT_FILENO;
            else if (&stream == &std::cerr)
                fd = STDERR_FILENO;
            else
                return false;

            return ::isatty(fd) != 0;
#endif
        }

        static const std::string &timestamp()
        {
            using namespace std::chrono;

            static std::time_t cached_second{};
            static std::string cached_text;
            static bool initialized = false;

            const auto now = system_clock::now();
            const std::time_t sec =
                system_clock::to_time_t(now);

            if (!initialized || sec != cached_second)
            {
                cached_second = sec;

                std::tm tm{};

#ifdef _WIN32
                localtime_s(&tm, &sec);
#else
                localtime_r(&sec, &tm);
#endif

                char buffer[20]{};
                std::strftime(
                    buffer, sizeof(buffer),
                    "%Y-%m-%d %H:%M:%S", &tm);

                cached_text = buffer;
                initialized = true;
            }

            return cached_text;
        }
    };
    // A no-op sink for disabled log macros.
    struct EmptyLogger
    {
        static constexpr void empty() noexcept {}
    };

}

// Compile-time filter threshold.
// Trace=0, Debug=1, Info=2, Warning=3, Error=4.
#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#ifndef LOG_LEVEL_TRACE
#define LOG_LEVEL_TRACE 0
#endif
#ifndef LOG_LEVEL_DEBUG
#define LOG_LEVEL_DEBUG 1
#endif
#ifndef LOG_LEVEL_INFO
#define LOG_LEVEL_INFO 2
#endif
#ifndef LOG_LEVEL_SUCCESS
#define LOG_LEVEL_SUCCESS 2
#endif
#ifndef LOG_LEVEL_PROGRESS
#define LOG_LEVEL_PROGRESS 2
#endif
#ifndef LOG_LEVEL_WARNING
#define LOG_LEVEL_WARNING 3
#endif
#ifndef LOG_LEVEL_ERROR
#define LOG_LEVEL_ERROR 4
#endif

#if LOG_LEVEL_TRACE >= LOG_LEVEL
#define LOG_TRACE(...) \
    Nilib::Logger::log(Nilib::LogLevel::Trace, true, __VA_ARGS__)
#else
#define LOG_TRACE(...) Nilib::EmptyLogger::empty()
#endif

#if LOG_LEVEL_DEBUG >= LOG_LEVEL
#define LOG_DEBUG(...) \
    Nilib::Logger::log(Nilib::LogLevel::Debug, true, __VA_ARGS__)
#else
#define LOG_DEBUG(...) Nilib::EmptyLogger::empty()
#endif

#if LOG_LEVEL_INFO >= LOG_LEVEL
#define LOG_INFO(...) \
    Nilib::Logger::log(Nilib::LogLevel::Info, true, __VA_ARGS__)
#else
#define LOG_INFO(...) Nilib::EmptyLogger::empty()
#endif

#if LOG_LEVEL_SUCCESS >= LOG_LEVEL
#define LOG_SUCCESS(...) \
    Nilib::Logger::log(Nilib::LogLevel::Success, true, __VA_ARGS__)
#else
#define LOG_SUCCESS(...) Nilib::EmptyLogger::empty()
#endif

#if LOG_LEVEL_PROGRESS >= LOG_LEVEL
#define LOG_PROGRESS(...) \
    Nilib::Logger::log(Nilib::LogLevel::Progress, true, __VA_ARGS__)
#else
#define LOG_PROGRESS(...) Nilib::EmptyLogger::empty()
#endif

#if LOG_LEVEL_WARNING >= LOG_LEVEL
#define LOG_WARNING(...) \
    Nilib::Logger::log(Nilib::LogLevel::Warning, true, __VA_ARGS__)
#else
#define LOG_WARNING(...) Nilib::EmptyLogger::empty()
#endif

#if LOG_LEVEL_ERROR >= LOG_LEVEL
#define LOG_ERROR(...) \
    Nilib::Logger::log(Nilib::LogLevel::Error, true, __VA_ARGS__)
#else
#define LOG_ERROR(...) Nilib::EmptyLogger::empty()
#endif

#endif // NILIB_LOGGING_H