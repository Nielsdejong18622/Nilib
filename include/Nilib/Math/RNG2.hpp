#ifndef _RNG2_H
#define _RNG2_H

#include <stdint.h>
#include <numeric>

namespace Nilib {

    // Uses wyhash64
    struct RNG {
        static uint64_t state;

        static void seed(uint64_t const seed) {
            state = seed;
        }

        static float prob() {
            return static_cast<float>(wyhash(&state)) / static_cast<float>(std::numeric_limits<uint64_t>::max());
        }

        static inline uint64_t wyhash(uint64_t *seed) {
            *seed += UINT64_C(0x60bee2bee120fc15);
            __uint128_t tmp;
            tmp = (__uint128_t)*seed * UINT64_C(0xa3b195354a39b70d);
            uint64_t m1 = (tmp >> 64) ^ tmp;
            tmp = (__uint128_t)m1 * UINT64_C(0x1b03738712fad5c9);
            uint64_t m2 = (tmp >> 64) ^ tmp;
            return m2;
        }

    };

//static RNG random = RNG::getInstance();
}

#endif