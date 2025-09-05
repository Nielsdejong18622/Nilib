#ifndef _STATS_H
#define _STATS_H

#include <stdint.h>
#include <ostream>
#include <iomanip>
#include <math.h>
#include <array>

namespace Nilib
{

    // Running statistics.
    // See https://www.johndcook.com/skewness_kurtosis.html.
    // See https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
    class RunningStats
    {
        float d_M1, d_M2, d_M3, d_M4, d_sum, d_max, d_min, d_lag;
        size_t d_n;

    public:
        RunningStats();
        void push(float const obs);
        void reset();

        inline size_t n() const { return d_n; }
        inline float mean() const { return d_M1; }
        inline float variance() const { return (d_n > 1) ? d_M2 / (d_n - 1.0) : 0.0; }
        inline float stddev() const { return std::sqrt(variance()); }
        inline float sum() const { return d_sum; }
        inline float min() const { return d_min; }
        inline float max() const { return d_max; }
        inline float lag() const { return d_lag; }
        inline float kurtosis() const { return static_cast<float>(d_n) * d_M4 / (d_M2 * d_M2) - 3.0; }
        inline float skewness() const { return std::sqrt(static_cast<float>(d_n)) * d_M3 / std::pow(d_M2, 1.5); }

        RunningStats &operator+=(RunningStats const &other);
        friend RunningStats operator+(RunningStats lhs, RunningStats const &rhs);
        friend std::ostream &operator<<(std::ostream &os, RunningStats const &stats);
    };

    class Reservoir
    {
        std::array<float, 1024> d_reservoir;
        size_t d_n = 0;

    public:
        void push(float const obs);
        void reset();
        inline size_t n() const { return d_n; }
        float mean() const;
        float sum() const;
        float variance() const;
        float stddev() const;

        float Q0();
        float Q1();
        float Q2();
        float Q3();
        float Q4();
        friend std::ostream &operator<<(std::ostream &os, Reservoir &stats);
    };

    RunningStats operator+(RunningStats lhs, RunningStats const &rhs);

    std::ostream &operator<<(std::ostream &os, Reservoir &stats);
    std::ostream &operator<<(std::ostream &os, RunningStats const &stats);
} // namespace Nilib

#endif