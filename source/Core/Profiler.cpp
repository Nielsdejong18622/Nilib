#include "Nilib/Core/Profiler.hpp"

#ifdef ENABLE_PROFILE
using namespace Nilib;

Profiler::Profiler()
    : d_function_name(""), d_file(""), d_line(0), d_start_timepoint(std::chrono::high_resolution_clock::now()), d_printondestruct(false)
{
}

Profiler::Profiler(const char *func_name, const char *file_name, int line_num)
    : d_function_name(func_name), d_file(file_name), d_line(line_num), d_start_timepoint(std::chrono::high_resolution_clock::now()), d_printondestruct(true)
{
}

Profiler::~Profiler()
{
    if (d_printondestruct)
    {
        auto end_timepoint = std::chrono::high_resolution_clock::now();
        std::chrono::duration<long long, std::nano> duration = end_timepoint - d_start_timepoint;
        printFormattedTime(duration);
    }
}

void Profiler::printFormattedTime(std::chrono::duration<long long, std::nano> const &duration) const
{
    auto duration_casted = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    LOG_DEBUG() << "PROFILED: __" << d_function_name << "__ in file " << d_file << ':' << d_line << " execution time: " << duration_casted << '\n';
}

void Profiler::reset()
{
    d_start_timepoint = std::chrono::high_resolution_clock::now();
}

size_t Profiler::getSeconds() const
{
    auto end_timepoint = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_timepoint - d_start_timepoint);
    return static_cast<size_t>(duration.count());
}

size_t Profiler::getMilliseconds() const
{
    auto end_timepoint = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_timepoint - d_start_timepoint);
    return static_cast<size_t>(duration.count());
}

#endif