#include "Nilib/Math/RNG.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Math/Stats.hpp"

int main()
{
    using namespace Nilib;
    Nilib::RNG::seed(126);

    LOG_DEBUG("Online statistics demo.");
    RunningStats stats, stats2;

    stats.push(5);
    stats.push(127);
    stats.push(-10);
    stats.push(-0);

    LOG_INFO("Stats:", stats);

    stats.reset();
    LOG_INFO("Stats:", stats);


    for (size_t sample = 0; sample < 100'000; ++sample)
    {
        //float obs = RNG::lognormal(10.0, 5.0);
        float obs = RNG::uniform(-20.0, 10.0);

        if (obs > 2.5)
            stats.push(obs);
        else 
            stats2.push(obs);
    } 
    LOG_INFO("Stats1:", stats);
    LOG_INFO("Stats2:", stats2);
    LOG_INFO("Stats1 + Stats2:", stats + stats2);
    
}