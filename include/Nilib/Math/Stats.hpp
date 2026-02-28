#ifndef _STATS_H
#define _STATS_H

#include <algorithm>
#include <array>
#include <iomanip>
#include <math.h>
#include <ostream>
#include <stdint.h>

namespace Nilib
{

// Running statistics.
// See https://www.johndcook.com/skewness_kurtosis.html.
// See https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
template <typename type = float> class RunningStats
{
    type d_M1, d_M2, d_M3, d_M4, d_sum, d_max, d_min, d_lag;
    size_t d_n;

  public:
    RunningStats()
    {
        reset();
    };

    void push(type const obs)
    {
        type delta, delta_n, delta_n2, term1;
        d_lag = obs;
        type d_n_f = static_cast<type>(d_n);
        type n1 = d_n_f;
        d_n++;

        delta = obs - d_M1;
        delta_n = delta / d_n_f;
        delta_n2 = delta_n * delta_n;
        term1 = delta * delta_n * n1;

        d_M1 += delta_n;
        d_M4 += term1 * delta_n2 * (d_n_f * d_n_f - 3 * d_n_f + 3) + 6 * delta_n2 * d_M2 - 4 * delta_n * d_M3;
        d_M3 += term1 * delta_n * (d_n_f - 2) - 3 * delta_n * d_M2;
        d_M2 += term1;

        d_max = (obs > d_max) ? obs : d_max;
        d_min = (obs < d_min) ? obs : d_min;
        d_sum += obs;
    }

    void reset()
    {
        d_M1 = d_M2 = d_M3 = d_M4 = d_sum = d_lag = 0;
        d_max = std::numeric_limits<type>::min();
        d_min = std::numeric_limits<type>::max();
        d_n = 0;
    }

    inline size_t n() const
    {
        return d_n;
    }
    inline type mean() const
    {
        return d_M1;
    }
    inline type variance() const
    {
        return (d_n > 1) ? d_M2 / (static_cast<type>(d_n) - 1.0) : 0.0;
    }
    inline type stddev() const
    {
        return std::sqrt(variance());
    }
    inline type sum() const
    {
        return d_sum;
    }
    inline type min() const
    {
        return d_min;
    }
    inline type max() const
    {
        return d_max;
    }
    inline type lag() const
    {
        return d_lag;
    }
    inline type kurtosis() const
    {
        return static_cast<type>(d_n) * d_M4 / (d_M2 * d_M2) - 3.0;
    }
    inline type skewness() const
    {
        return std::sqrt(static_cast<type>(d_n)) * d_M3 / std::pow(d_M2, 1.5);
    }

    RunningStats &operator+=(RunningStats const &other)
    {
        if (other.d_n == 0)
            return *this;
        if (d_n == 0)
            return (*this = other);

        size_t const n1 = d_n;
        size_t const n2 = other.d_n;
        size_t const n_12 = n1 + n2;
        type const delta = other.d_M1 - d_M1;
        type const delta2 = delta * delta;
        type const delta3 = delta2 * delta;
        type const delta4 = delta3 * delta;
        type const n1n2 = static_cast<type>(n1 * n2);
        type const n1f = static_cast<type>(n1);
        type const n2f = static_cast<type>(n2);
        type const nf = static_cast<type>(n_12);

        d_sum += other.d_sum;
        d_max = std::max(d_max, other.d_max);
        d_min = std::min(d_min, other.d_min);
        d_lag = other.d_lag;

        type newM1 = d_M1 + delta * n2f / nf;

        type m2 = d_M2 + other.d_M2 + delta2 * n1n2 / nf;
        type m3 = d_M3 + other.d_M3 + delta3 * n1n2 * (n1f - n2f) / (nf * nf) +
                  3.0f * delta * (n1f * other.d_M2 - n2f * d_M2) / nf;

        type m4 = d_M4 + other.d_M4 + delta4 * n1n2 * (n1f * n1f - n1f * n2f + n2f * n2f) / (nf * nf * nf) +
                  6.0f * delta2 * (n1f * n1f * other.d_M2 + n2f * n2f * d_M2) / (nf * nf) +
                  4.0f * delta * (n1f * other.d_M3 - n2f * d_M3) / nf;

        d_M1 = newM1;
        d_M2 = m2;
        d_M3 = m3;
        d_M4 = m4;
        d_n = n_12;

        return *this;
    }
    friend RunningStats operator+(RunningStats lhs, RunningStats const &rhs)
    {
        lhs += rhs;
        return lhs;
    }
    friend std::ostream &operator<<(std::ostream &os, RunningStats const &stats)
    {
        if (stats.d_n > 0)
            return os << std::fixed << std::setprecision(2) << "Mean:" << stats.mean() << '(' << stats.stddev()
                      << ") Min:" << stats.min() << " Max:" << stats.max() << " skew:" << stats.skewness()
                      << " kurtosis:" << stats.kurtosis() << " n:" << stats.n() << " sum:" << stats.sum()
                      << " lag:" << stats.lag();
        return os << "Stat is empty!";
    }
};

} // namespace Nilib

#endif