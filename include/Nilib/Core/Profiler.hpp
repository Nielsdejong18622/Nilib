#ifndef _PROFILER_H
#define _PROFILER_H

#include "Nilib/Logger/Log.hpp"
#include <chrono>
#include <string_view>

namespace Nilib
{

    class Profiler
    {
        void printFormattedTime(std::chrono::duration<long long, std::nano> const &duration) const;

        std::string_view const d_function_name;
        std::string_view const d_file;
        size_t d_line;
        std::chrono::time_point<std::chrono::high_resolution_clock> d_start_timepoint;
        bool d_printondestruct;

    public:
        Profiler();
        Profiler(std::string_view const func_name, std::string_view const file_name, size_t const line_num);
        Profiler(Profiler const &) = delete;
        Profiler &operator=(Profiler const &) = delete;
        ~Profiler();

        void reset();

        std::chrono::seconds getSeconds() const;
        std::chrono::milliseconds getMilliseconds() const;
        std::chrono::nanoseconds getNanoseconds() const;
    };

    using Timer = Profiler;

}; // Namespace.

#ifndef ENABLE_PROFILE
#define PROFILE_FUNCTION() (void)(0)
#define PROFILE_SCOPE_LINE2(name, line) (void)(0)
#define PROFILE_SCOPE_LINE(name, line) (void)(0)
#define PROFILE_SCOPE(name) (void)(0)
#define PROFILE_FUNCTION() (void)(0)
#else
// Macro magic.
#define PROFILE_SCOPE_LINE2(name, line) ::Nilib::Profiler timer##line(name, __FILE__, line)
#define PROFILE_SCOPE_LINE(name, line) PROFILE_SCOPE_LINE2(name, line)
#define PROFILE_SCOPE(name) PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__func__)
#endif // ENABLE_PROFILE
#endif