#ifndef _STATS_H
#define _STATS_H

#include <stdint.h>
#include <ostream>
#include <iomanip>
#include <math.h>

namespace Nilib
{

    // Running statistics. 
    // See https://www.johndcook.com/skewness_kurtosis.html
    class RunningStats
    {
        float M1, M2, M3, M4, Msum, Mmax, Mmin;
        size_t d_n;

    public:
        RunningStats() { reset(); } 

        void push(float const obs) 
        {
            float delta, delta_n, delta_n2, term1;
            size_t n1 = d_n;
            
            d_n++;

            delta = obs - M1;
            delta_n = delta / d_n;
            delta_n2 = delta_n * delta_n;
            term1 = delta * delta_n * n1;

            M1 += delta_n; // The mean.
            M4 += term1 * delta_n2 * (d_n * d_n - 3 * d_n + 3) + 6 * delta_n2 * M2 - 4 * delta_n * M3;
            M3 += term1 * delta_n * (d_n - 2) - 3 * delta_n * M2;
            M2 += term1;

            Mmax = (obs > Mmax) ? obs : Mmax;
            Mmin = (obs < Mmin) ? obs : Mmin;
            Msum += obs;
        }

        void reset() {
            M1 = M2 = M3 = M4 = Msum = 0;
            Mmax = std::numeric_limits<float>::min();
            Mmin = std::numeric_limits<float>::max();
            d_n = 0;
        }

        size_t n() const { return d_n; }
        float mean() const { return M1;}
        float variance() const { return M2/(d_n - 1.0);}
        float stddev() const { return std::sqrt(variance());}
        float skewness() const { return std::sqrt(static_cast<float>(d_n)) * M3 / std::pow(M2, 1.5);}
        float sum() const { return Msum;}
        float min() const { return Mmin;}
        float max() const { return Mmax;}
        float kurtosis() const { return static_cast<float>(d_n) * M4 / (M2 * M2) - 3.0;}

        friend std::ostream &operator<<(std::ostream &os, RunningStats const &stats) {
            return os << std::fixed << std::setprecision(4)  
                      << "Mean:" << stats.mean() 
                      << '(' << stats.stddev()
                      << ") Min:" << stats.min()  
                      << " Max:" << stats.max() 
                      << " sum:" << stats.sum() 
                      << " skew:" << stats.skewness() 
                      << " kurtosis:" << stats.kurtosis() 
                      << " n:" << stats.n();
        }
    };




} // namespace Nilib

#endif