#include "Nilib/Core/Profiler.hpp"

#include <fmt/core.h>

using namespace Nilib;

int main()
{
    PROFILE_FUNCTION();
    for (size_t i = 0; i < 15; i++)
    {
        PROFILE_SCOPE(fmt::format("Loop {}", i).c_str());
        LOG_INFO() << "Loop " << i << '\n';
        Sleep(40 * i);
    }

    // Destructors are called in reverse order of construction.
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
}