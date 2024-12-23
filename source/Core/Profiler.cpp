#include "Nilib/Core/Profiler.hpp"


#ifdef ENABLE_PROFILE
using namespace Nilib;

Profiler::Profiler(const char* func_name, const char* file_name, int line_num)
    : d_function_name(func_name), d_file(file_name), d_line(line_num), d_start_timepoint(std::chrono::high_resolution_clock::now())
{
}

Profiler::~Profiler() 
{
    auto end_timepoint = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> duration = end_timepoint - d_start_timepoint;
    printFormattedTime(duration);
}

void Profiler::printFormattedTime(std::chrono::duration<long long, std::nano> const &duration) const 
{
    auto duration_casted = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    LOG_DEBUG() << "PROFILED: __" << d_function_name << "__ in file " << d_file << ':' << d_line << " execution time: " << duration_casted << '\n';
}
#endif