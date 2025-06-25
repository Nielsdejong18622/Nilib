#include "Nilib/Math/RNG.hpp"
#include "Nilib/Math/Stats.hpp"
#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Core/Assert.hpp"

using namespace Nilib;

int main()
{
    // Set a seed.
    RNG::seed(127);

    // Generate a number between 0-1.
    auto gen = RNG::prob();

    // Generate a lognormal number.
    RNG::lognormal(2.38, 0.683);
    RNG::weibull(2.38, 0.683);

    for (size_t test = 0; test < 1; ++test)
    {
        PROFILE_SCOPE("Testing RNG::normal(20,0, 10.0)");
        Nilib::RunningStats probstat;
        for (size_t i = 0; i < 10'000'000; i++)
            probstat.push(RNG::normal(20.0, 10.0));
        LOG_INFO("Observed RNG::normal(20,0, 10.0)", probstat);
    }

    for (size_t test = 0; test < 1; ++test)
    {
        PROFILE_SCOPE("Testing RNG::index(10)");
        Nilib::RunningStats probstat;
        for (size_t i = 0; i < 10'000'000; i++)
            probstat.push(RNG::index(10));
        LOG_INFO("Observed RNG::index(10):", probstat);
    }

    Nilib::RNG::seed(127);

    for (size_t test = 0; test < 1; ++test)
    {
        PROFILE_SCOPE("Testing RNG::normal(0.0, 1.0)");
        Nilib::RunningStats probstat;
        for (size_t i = 0; i < 10'000'000; i++)
            probstat.push(Nilib::RNG::normal());
        LOG_INFO("Observed RNG::normal(0.0, 1.0):", probstat);
    }

    
    for (size_t test = 0; test < 1; ++test)
    {
        PROFILE_SCOPE("Testing RNG::rand()");
        Nilib::RunningStats probstat;
        for (size_t i = 0; i < 10'000'000; i++)
            probstat.push(Nilib::RNG::rand());
        LOG_INFO("Observed RNG::rand():", probstat);
    }
}