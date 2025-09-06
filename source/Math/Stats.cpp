#include "Nilib/Math/Stats.hpp"
#include "Nilib/Math/RNG.hpp"

using namespace Nilib;

RunningStats::RunningStats()
{
    reset();
}

void RunningStats::reset()
{
    d_M1 = d_M2 = d_M3 = d_M4 = d_sum = d_lag = 0;
    d_max = std::numeric_limits<type>::min();
    d_min = std::numeric_limits<type>::max();
    d_n = 0;
}

void RunningStats::push(type const obs)
{
    type delta, delta_n, delta_n2, term1;
    d_lag = obs;
    size_t n1 = d_n;

    d_n++;

    delta = obs - d_M1;
    delta_n = delta / d_n;
    delta_n2 = delta_n * delta_n;
    term1 = delta * delta_n * n1;

    d_M1 += delta_n;
    d_M4 += term1 * delta_n2 * (d_n * d_n - 3 * d_n + 3) + 6 * delta_n2 * d_M2 - 4 * delta_n * d_M3;
    d_M3 += term1 * delta_n * (d_n - 2) - 3 * delta_n * d_M2;
    d_M2 += term1;

    d_max = (obs > d_max) ? obs : d_max;
    d_min = (obs < d_min) ? obs : d_min;
    d_sum += obs;
}

RunningStats &RunningStats::operator+=(RunningStats const &other)
{
    if (other.d_n == 0)
        return *this;
    if (d_n == 0)
        return (*this = other);

    size_t const n1 = d_n;
    size_t const n2 = other.d_n;
    size_t const n = n1 + n2;
    type const delta = other.d_M1 - d_M1;
    type const delta2 = delta * delta;
    type const delta3 = delta2 * delta;
    type const delta4 = delta3 * delta;
    type const n1n2 = static_cast<type>(n1 * n2);
    type const n1f = static_cast<type>(n1);
    type const n2f = static_cast<type>(n2);
    type const nf = static_cast<type>(n);

    d_sum += other.d_sum;
    d_max = std::max(d_max, other.d_max);
    d_min = std::min(d_min, other.d_min);
    d_lag = other.d_lag;

    type newM1 = d_M1 + delta * n2f / nf;

    type m2 = d_M2 + other.d_M2 + delta2 * n1n2 / nf;
    type m3 = d_M3 + other.d_M3 + delta3 * n1n2 * (n1f - n2f) / (nf * nf) + 3.0f * delta * (n1f * other.d_M2 - n2f * d_M2) / nf;

    type m4 = d_M4 + other.d_M4 + delta4 * n1n2 * (n1f * n1f - n1f * n2f + n2f * n2f) / (nf * nf * nf) + 6.0f * delta2 * (n1f * n1f * other.d_M2 + n2f * n2f * d_M2) / (nf * nf) + 4.0f * delta * (n1f * other.d_M3 - n2f * d_M3) / nf;

    d_M1 = newM1;
    d_M2 = m2;
    d_M3 = m3;
    d_M4 = m4;
    d_n = n;

    return *this;
}

// Reservoir.

void Reservoir::push(float const obs)
{
    if (d_n < d_reservoir.size())
        d_reservoir[d_n] = obs;
    else
    {
        size_t rindex = Nilib::RNG::index(d_n);
        if (rindex < d_reservoir.size())
        {
            d_reservoir[rindex] = obs;
        }
        // else discard.
    }
    d_n++;
}

void Reservoir::reset()
{
    d_n = 0;
}

float Reservoir::sum() const
{
    CORE_ASSERT(d_n > 0);
    return std::accumulate(d_reservoir.begin(), d_reservoir.begin() + std::min(d_n, d_reservoir.size()), 0.0f);
}

float Reservoir::mean() const
{
    CORE_ASSERT(d_n > 0);
    float mean = 0.0f;
    for (size_t i = 0; i < std::min(d_n, d_reservoir.size()); i++)
        mean += d_reservoir[i] / std::min(d_n, d_reservoir.size());
    return mean;
}

float Reservoir::Q0()
{
    CORE_ASSERT(d_n > 0);
    std::sort(d_reservoir.begin(), d_reservoir.end());
    return d_reservoir[0];
}

float Reservoir::Q1()
{
    CORE_ASSERT(d_n > 0);
    std::sort(d_reservoir.begin(), d_reservoir.end());
    return d_reservoir[d_reservoir.size() / 4];
}
float Reservoir::Q2()
{
    CORE_ASSERT(d_n > 0);
    std::sort(d_reservoir.begin(), d_reservoir.end());
    return d_reservoir[d_reservoir.size() / 2];
}

float Reservoir::Q3()
{
    CORE_ASSERT(d_n > 0);
    std::sort(d_reservoir.begin(), d_reservoir.end());
    return d_reservoir[3 * d_reservoir.size() / 4];
}

float Reservoir::Q4()
{
    CORE_ASSERT(d_n > 0);
    std::sort(d_reservoir.begin(), d_reservoir.end());
    return d_reservoir[d_reservoir.size() - 1];
}

float Reservoir::variance() const
{
    float mu = mean();
    float std_dev = 0.0f;
    for (size_t i = 0; i < std::min(d_n, d_reservoir.size()); ++i)
        std_dev += (d_reservoir[i] - mu) * (d_reservoir[i] - mu);
    std_dev /= std::min(d_n, d_reservoir.size());

    return std_dev;
};

float Reservoir::stddev() const
{
    return std::sqrt(variance());
};

// In OnlineStatistics.cpp or RunningStats.cpp
Nilib::RunningStats Nilib::operator+(Nilib::RunningStats lhs, Nilib::RunningStats const &rhs)
{
    lhs += rhs;
    return lhs;
}

std::ostream &Nilib::operator<<(std::ostream &os, RunningStats const &stats)
{
    if (stats.d_n > 0)
        return os << std::fixed << std::setprecision(2)
                  << "Mean:" << stats.mean()
                  << '(' << stats.stddev()
                  << ") Min:" << stats.min()
                  << " Max:" << stats.max()
                  << " skew:" << stats.skewness()
                  << " kurtosis:" << stats.kurtosis()
                  << " n:" << stats.n()
                  << " sum:" << stats.sum()
                  << " lag:" << stats.lag();
    return os << "Stat is empty!";
}

std::ostream &Nilib::operator<<(std::ostream &os, Reservoir &stats)
{
    if (stats.d_n > 0)
        return os << std::fixed << std::setprecision(2)
                  << "Mean:" << stats.mean()
                  << '(' << stats.stddev()
                  << ") Min:" << stats.Q0()
                  << " Q1:" << stats.Q1()
                  << " Q2:" << stats.Q2()
                  << " Q3:" << stats.Q3()
                  << " Max:" << stats.Q4()
                  << " n:" << stats.n()
                  << " sum:" << stats.sum();
    return os << "Reservoir is empty!";
}
