#ifndef _LOGGING2_H
#define _LOGGING2_H

#include <mutex>
#include <iostream>
#include <string>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <chrono>

namespace Nilib
{

    enum class LogLevel : unsigned int
    {
        Info = 0,     // Default
        Success = 2,  // ON SUCCESS
        Warning = 14, // WARNING
        Error = 4,    // ERROR
        Debug = 8,    // DEBUG
        Trace = 8,    // Most detailed debug.
        Progress = 0  // To show to the end user.
    };

    // Meta programming.
    template <typename T>
    constexpr bool is_range =
        requires(T t) {
            std::begin(t);
            std::end(t);
        } && !std::is_convertible_v<T, const char *> && // exclude char*
        !std::is_convertible_v<T, std::string_view> &&  // exclude std::string_view
        !std::is_same_v<std::decay_t<T>, std::string>;  // exclude std::string

    // Disallows user to do constructs like LOG_DEBUG("Some text") << "Some more text!\n";
    struct DisallowedLogger
    {
        static DisallowedLogger &empty()
        {
            static DisallowedLogger instance;
            return instance;
        }
    };

    // When LOGGING is disabled, we replace all LOG_DEBUG() << "test1" << 2; with EmptyLogger() << "test1" << 2;
    struct EmptyLogger
    {
        static EmptyLogger &empty()
        {
            static EmptyLogger instance;
            return instance;
        }

        // Should be compiled away.
        template <typename T>
        EmptyLogger &operator<<(T const &)
        {
            return EmptyLogger::empty();
        }
    };

    class Logger
    {
    private:
        // Recursive log printing of template arguments
        template <typename T, typename... Args>
        DisallowedLogger &log_internal(T &&first, Args &&...args)
        {
            // std::lock_guard<std::mutex> lock(d_mutex);
            if (!d_stream)
                return DisallowedLogger::empty();

            // Logging of custom types such as ranges:
            if constexpr (is_range<T>)
            {
                *d_stream << '[';
                bool first_elem = true;
                for (auto &&elem : first)
                {
                    if (!first_elem)
                        *d_stream << ", ";
                    *d_stream << elem;
                    first_elem = false;
                }
                *d_stream << ']';
                // Or exceptions.
            }
            else if constexpr (std::is_base_of_v<std::exception, std::remove_cvref_t<T>>)
            {
                *d_stream << typeid(first).name() << ' ' << first.what();
            }
            else
            {
                *d_stream << first; // Print the first argument
            }

            if constexpr (sizeof...(args) > 0)
            {                                                     // Check if there are more arguments
                *this << d_sep;                                   // Print separator
                return log_internal(std::forward<Args>(args)...); // Template recurse
            }
            else
            {
                *this << d_end;
                return DisallowedLogger::empty();
            }
        }

    public:
        static Logger &default_logger()
        {
            static Logger instance(std::cout, '\n', ' '); // C++11 guarantees thread-safe initialization
            return instance;
        }

        template <typename T>
        Logger &operator<<(T const &msg)
        {
            if (d_stream)
                *d_stream << msg;
            return *this;
        }

        // Base case for empty variadic
        Logger &log()
        {
            auto now = std::chrono::system_clock::now();
            std::string formatted_time = std::format("{:%Y-%m-%d %H:%M:%S}", now);
            if (d_stream)
                *d_stream << '[' << formatted_time << "]:";
            return *this;
        }

        // Set the upcoming level and other options.
        Logger &set(LogLevel level, char end = '\n', char sep = ' ')
        {
            // Set the text color based on the platform
#ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (level != LogLevel::Info)
                SetConsoleTextAttribute(hConsole, static_cast<WORD>(level));
            else
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
#ifdef _LINUX
            switch (level)
            {
            case LogLevel::Error:
                (*d_stream) << "\033[31m";
                break;
            case LogLevel::Warning:
                (*d_stream) << "\033[33m";
                break;
            case LogLevel::Success:
                (*d_stream) << "\033[32m";
                break;
            case LogLevel::Debug:
                (*d_stream) << "\033[90m";
                break;
            default:
                (*d_stream) << "\033[0m";
                break;
            }
#endif
            d_end = end;
            d_sep = sep;

            return *this;
        }

        // Recursive log printing of template arguments
        template <typename T, typename... Args>
        DisallowedLogger &log(T &&first, Args &&...args)
        {
            auto now = std::chrono::system_clock::now();
            std::string formatted_time = std::format("{:%Y-%m-%d %H:%M:%S}", now);
            *d_stream << '[' << formatted_time << "]:";
            return log_internal(std::forward<T>(first), std::forward<Args>(args)...); // Template recurse
        }

        // Redirects the stream.
        void redirect(std::ostream &other)
        {
            d_stream = &other;
        }

        Logger() : d_stream(&std::cout), d_end('\n'), d_sep(' ')
        {
        }

        Logger(Logger const &other) : d_stream(other.d_stream), d_end('\n'), d_sep(' ')
        {
        }

        Logger(std::ostream &os, char end = '\n', char sep = ' ') : d_stream(&os), d_end(end), d_sep(sep)
        {
        }

        ~Logger()
        {
            set(Nilib::LogLevel::Info);
            flush();
        };

        void flush()
        {
            d_stream->flush();
        }

    private:
        // std::mutex d_mutex;     // protects log output
        std::ostream *d_stream = &std::cout;
        char d_end = '\n';
        char d_sep = ' ';
    };
    // Logger needs to support:
    // Color. Timestamp (custom formatting).
    // Different Log levels.
    // Some sort of dynamic switch to (rotating) file.
    // Callable from anywhere in the code.

    // Logger needs to end with '\n'.
    // Logger_r needs to end with '\r'.

};

// Macro's
// Only LOG_LEVEL_TYPES equal or higher than the current LOG_LEVEL are printed.
#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

// #define REGISTER_LOGGER(loggername, LOGGER_PTR) ::Nilib::LoggerPool::instance().register_logger(loggername, LOGGER_PTR)
#define REGISTER_LOGGER(loggername, LOGGER_PTR) ;

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

#if (LOG_LEVEL_TRACE >= LOG_LEVEL)
#define LOG_TRACE(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Trace).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_TRACE(...) Nilib::EmptyLogger::empty()
#endif

#if (LOG_LEVEL_DEBUG >= LOG_LEVEL)
#define LOG_DEBUG(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Debug).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_DEBUG(...) Nilib::EmptyLogger::empty()
#endif

#if (LOG_LEVEL_INFO >= LOG_LEVEL)
#define LOG_INFO(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Info).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_INFO(...) Nilib::EmptyLogger::empty()
#endif

#if (LOG_LEVEL_WARNING >= LOG_LEVEL)
#define LOG_WARNING(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Warning).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_WARNING(...) Nilib::EmptyLogger::empty()
#endif

#if (LOG_LEVEL_ERROR >= LOG_LEVEL)
#define LOG_ERROR(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Error).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_ERROR(...) Nilib::EmptyLogger::empty()
#endif

#if (LOG_LEVEL_SUCCESS >= LOG_LEVEL)
#define LOG_SUCCESS(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Success).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_SUCCESS(...) Nilib::EmptyLogger::empty()
#endif

#if (LOG_LEVEL_PROGRESS >= LOG_LEVEL)
#define LOG_PROGRESS(...) (Nilib::Logger::default_logger().set(Nilib::LogLevel::Progress).log(__VA_OPT__(__VA_ARGS__)))
#else
#define LOG_PROGRESS(...) Nilib::EmptyLogger::empty()
#endif

#endif // LOGGING2_H