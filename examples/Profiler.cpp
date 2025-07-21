#include "Nilib/Core/Profiler.hpp"

#include <format>

using namespace Nilib;

int main()
{
    PROFILE_FUNCTION();
    for (size_t i = 0; i < 15; i++)
    {
        PROFILE_SCOPE(std::format("Loop {}", i).c_str());
        LOG_INFO() << "Loop " << i << '\n';
#ifdef WIN_32
        Sleep(40 * i);
#endif
    }

    // Destructors are called in reverse order of construction.
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
    PROFILE_FUNCTION();
}