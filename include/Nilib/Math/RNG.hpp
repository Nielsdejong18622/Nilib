#ifndef _RNG2_H
#define _RNG2_H

#include <stdint.h>
#include <math.h>
#include <numeric>
#include <vector>
#include <chrono>

#include "Nilib/Core/Assert.hpp"

namespace Nilib
{

    // Uses wyhash64
    template <size_t stream = 0>
    class RAN
    {
        static uint64_t state;

    public:
        static inline void seed(uint64_t const seed)
        {
            state = seed;
        }

        // Seed with current time.
        static inline void seed()
        {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = now.time_since_epoch();

            state = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        }

        // Returns a probability [0, 1)
        template <typename scalar = float>
        static inline float prob()
        {
            // double const _wynorm = 1.0 / (1ull << 52);
            // return static_cast<scalar>((wyhash(&state) >> 12) * _wynorm);
            return static_cast<scalar>(static_cast<float>(wyhash(&state)) /
                                       static_cast<float>(std::numeric_limits<uint64_t>::max()));
        }

        static inline float rounded_uniform(float const min, float const max, int const digits = 2)
        {
            float const multiplier = std::pow(10.0, digits);
            return std::ceil(uniform(min, max) * multiplier) / multiplier;
        }

        // Uniform random variable between [min, max]
        template <typename scalar = float>
        static inline float uniform(scalar const min, scalar const max)
        {
            ASSERT(max >= min, "Degenerate uniform!");
            return min + RAN<stream>::prob() * (max - min);
        }

        // Uniform random variable between [min, max]
        static inline int uniform(int const min, int const max)
        {
            ASSERT(max >= min, "Degenerate uniform!");
            return min + RAN<stream>::index(max - min);
        }
        // Uniform random variable between [min, max]
        static inline size_t uniform(size_t const min, size_t const max)
        {
            ASSERT(max >= min, "Degenerate uniform!");
            return min + RAN<stream>::index(max - min);
        }

        static inline uint64_t rand()
        {
            return wyhash(&state);
        }

        // Returns a random index [0, weights.size()), according to weights.
        template <typename Container>
        static inline size_t index_weights(Container &weights)
        {
            CORE_ASSERT(weights.size() > 0);
            auto total_weight = 0.0, cumulative = 0.0;
            for (auto &&w : weights)
                total_weight += w;

            auto idx = RAN<stream>::prob() * total_weight;

            for (size_t i = 0; i < weights.size(); ++i)
            {
                cumulative += weights[i];
                if (idx < cumulative)
                    return i;
            }
            // Due to floating-point precision, fallback to last index
            return weights.size() - 1;
        }

        // Returns a random index [0, size)
        static inline size_t index(size_t const size)
        {
            if (size == 0)
                return 0;
            return wyhash(&state) % size;
        }

        static inline uint64_t wyhash(uint64_t *seed)
        {
            *seed += UINT64_C(0x60bee2bee120fc15);
            __uint128_t tmp;
            tmp = (__uint128_t)*seed * UINT64_C(0xa3b195354a39b70d);
            uint64_t m1 = (tmp >> 64) ^ tmp;
            tmp = (__uint128_t)m1 * UINT64_C(0x1b03738712fad5c9);
            uint64_t m2 = (tmp >> 64) ^ tmp;
            return m2;
        }

        // Generates a value N(0,1)
        template <typename scalar = float>
        static inline scalar normal() { return normal(0.0, 1.0); }

        // convert any 64 bit pseudo random numbers to APPROXIMATE Gaussian distribution. It can be combined with wyrand, wyhash64 or wyhash.
        static inline double wy2gau(uint64_t r)
        {
            const double _wynorm = 1.0 / (1ull << 20);
            return ((r & 0x1fffff) + ((r >> 21) & 0x1fffff) + ((r >> 42) & 0x1fffff)) * _wynorm - 3.0;
        }

        template <typename scalar = float>
        static inline scalar normal(scalar const mean, scalar const var)
        {
            CORE_ASSERT(var > static_cast<scalar>(0.0))
            // Box-Muller
            scalar U = prob();
            scalar V = prob();
            scalar constexpr PI = 3.1415926;
            return std::sqrt(-2.0 * std::log(U)) * std::cos(2 * PI * V) * std::sqrt(var) + mean;

            // Irwin-Hall relies on CLT.
            scalar d1a = prob(), d2a = prob(), d3a = prob();
            scalar d1b = prob(), d2b = prob(), d3b = prob();
            scalar d1c = prob(), d2c = prob(), d3c = prob();
            scalar d1d = prob(), d2d = prob(), d3d = prob();
            scalar sum = d1a + d1b + d1c + d1d +
                         d2a + d2b + d2c + d2d +
                         d3a + d3b + d3c + d3d - 6.0;
            return sum;
        }

        template <typename scalar = float>
        static inline scalar lognormal(scalar const mean, scalar const var)
        {
            // TODO: Check lognormal!
            return std::log(normal(mean, var));
        }

        template <typename scalar = float>
        static inline scalar weibull(scalar const scale, scalar const shape)
        {
            // TODO: Check !
            return scale * std::pow(-1 * std::log(1 - RAN<stream>::prob()), 1.0f / shape);
        }

        // Shuffles container between [low, high), inplace.
        template <typename Container>
        static inline void shuffle(Container &container, size_t const low, size_t const high)
        {
            // Length of the subarray to shuffle.
            size_t const nobs = high - low;
            // Shuffle in place. (Fisher-Yates shuffle)
            for (size_t idx = low; idx < nobs - 1; ++idx)
            {
                size_t jdx = RAN<stream>::index(nobs) + low;
                std::swap(container[idx], container[jdx]);
            }
        }

        template <typename Container>
        static inline void shuffle(Container &container)
        {
            shuffle(container, 0, container.size());
        }

        template <typename Container>
        static inline auto sample(Container const &container) -> decltype(container[0])
        {
            CORE_ASSERT(container.size() > 0);
            return container[RAN<stream>::index(container.size())];
        }

        template <typename Container, typename Weights>
        static inline auto sample(Container const &container, Weights const &weights) -> decltype(container[0])
        {
            auto total_w = 0;
            for (auto &&w : weights)
                total_w += w;

            return container[RAN<stream>::index(container.size())];
        }

        // Uniform sample from an initializer list
        template <typename T>
        static inline T sample(std::initializer_list<T> list)
        {
            CORE_ASSERT(list.size() > 0);
            std::vector<T> vec(list);
            return vec[RAN<stream>::index(vec.size())];
        }
    };

    using RNG = RAN<0>;
}

#endif
