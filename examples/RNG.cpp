#include "Nilib/Math/RNG.hpp"
#include "Nilib/Math/Stats.hpp"
#include "Nilib/Core/Profiler.hpp"

using namespace Nilib;

int main()
{
    // Set a seed.
    RNG::seed(127);

    // Generate a number between 0-1.
    RNG::prob();

    for (size_t test = 0; test < 1; ++test)
    {
        PROFILE_SCOPE("Testing probs");
        Nilib::RunningStats probstat;
        for (size_t i = 0; i < 10'000'000; i++)
            probstat.push(RNG::normal(0.0, 1.0));
        LOG_INFO("Observed Probs:", probstat);
    }
    
    Nilib::RNG::seed(127);

    for (size_t test = 0; test < 1; ++test)
    {
        PROFILE_SCOPE("Testing probs");
        Nilib::RunningStats probstat;
        for (size_t i = 0; i < 10'000'000; i++)
            probstat.push(Nilib::RNG::normal());
        LOG_INFO("Observed Probs:", probstat);
    }

}