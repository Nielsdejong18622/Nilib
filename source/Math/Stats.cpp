#include "Nilib/Math/Stats.hpp"

using namespace Nilib;

RunningStats::RunningStats()
{
    reset();
}

void RunningStats::reset()
{
    d_M1 = d_M2 = d_M3 = d_M4 = d_sum = d_lag = 0;
    d_max = std::numeric_limits<float>::min();
    d_min = std::numeric_limits<float>::max();
    d_n = 0;
}

void RunningStats::push(float const obs)
{
    float delta, delta_n, delta_n2, term1;
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


RunningStats& RunningStats::operator+=(RunningStats const &other)
{
    if (other.d_n == 0) return *this;
    if (d_n == 0) return (*this = other);

    const size_t n1 = d_n;
    const size_t n2 = other.d_n;
    const size_t n = n1 + n2;

    const float delta = other.d_M1 - d_M1;
    const float delta2 = delta * delta;
    const float delta3 = delta2 * delta;
    const float delta4 = delta3 * delta;

    const float n1n2 = static_cast<float>(n1 * n2);
    const float n1f = static_cast<float>(n1);
    const float n2f = static_cast<float>(n2);
    const float nf = static_cast<float>(n);

    d_sum += other.d_sum;
    d_max = std::max(d_max, other.d_max);
    d_min = std::min(d_min, other.d_min);
    d_lag = other.d_lag;

    float newM1 = d_M1 + delta * n2f / nf;

    float m2 = d_M2 + other.d_M2 + delta2 * n1n2 / nf;
    float m3 = d_M3 + other.d_M3
        + delta3 * n1n2 * (n1f - n2f) / (nf * nf)
        + 3.0f * delta * (n1f * other.d_M2 - n2f * d_M2) / nf;

    float m4 = d_M4 + other.d_M4
        + delta4 * n1n2 * (n1f * n1f - n1f * n2f + n2f * n2f) / (nf * nf * nf)
        + 6.0f * delta2 * (n1f * n1f * other.d_M2 + n2f * n2f * d_M2) / (nf * nf)
        + 4.0f * delta * (n1f * other.d_M3 - n2f * d_M3) / nf;

    d_M1 = newM1;
    d_M2 = m2;
    d_M3 = m3;
    d_M4 = m4;
    d_n = n;

    return *this;
}


// In OnlineStatistics.cpp or RunningStats.cpp
Nilib::RunningStats Nilib::operator+(Nilib::RunningStats lhs, Nilib::RunningStats const &rhs)
{
    lhs += rhs;
    return lhs;
}

std::ostream &Nilib::operator<<(std::ostream &os, RunningStats const &stats)
{
    return os << std::fixed //<< std::setprecision(4)
              << "Mean:" << stats.mean()
              << '(' << stats.stddev()
              << ") Min:" << stats.min()
              << " Max:" << stats.max()
              << " skew:" << stats.skewness()
              << " kurtosis:" << stats.kurtosis()
              << " n:" << stats.n()
              << " sum:" << stats.sum()
              << " lag:" << stats.lag();
}