#ifndef _PROFILER_H
#define _PROFILER_H

#include "Nilib/Logger/Log.hpp"
#include <chrono>

namespace Nilib
{

    class Profiler
    {
        void printFormattedTime(std::chrono::duration<long long, std::nano> const &duration) const;

        const char *d_function_name;
        const char *d_file;
        int d_line;
        std::chrono::time_point<std::chrono::high_resolution_clock> d_start_timepoint;
        bool d_printondestruct;

    public:
        Profiler();
        Profiler(const char *func_name, const char *file_name, int line_num);
        ~Profiler();

        void reset();

        size_t getSeconds() const;
        size_t getMilliseconds() const;
    };

    using Timer = Profiler;

}; // Namespace.

#ifndef ENABLE_PROFILE
#define PROFILE_FUNCTION() (void)(0)
#else
   // Marco magic.
#define PROFILE_SCOPE_LINE2(name, line) ::Nilib::Profiler timer##line(name, __FILE__, line)
#define PROFILE_SCOPE_LINE(name, line) PROFILE_SCOPE_LINE2(name, line)
#define PROFILE_SCOPE(name) PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__func__)
#endif // ENABLE_PROFILE
#endif