#ifndef _RNG_H
#define _RNG_H

#include <random>

class RNG {

public:
    // Get the singleton instance
    static RNG& getInstance() {
        static RNG instance;
        return instance;
    }

    static void seed(size_t const seed) {
        RNG::getInstance().gen = std::mt19937(seed);
    }

    static size_t index(size_t const size) {
        std::uniform_int_distribution<size_t> dis(0, size - 1);
        return dis(RNG::getInstance().gen);
    }
    
    // Generate a random probability between 0 and 1
    static float prob() {
        std::uniform_real_distribution<float> dis(0.0, 1.0);
        return dis(RNG::getInstance().gen);
    }


    // Generate a uniform value between min and max
    static double uniform(double const min, double const max) {
        std::uniform_real_distribution<double> dis(min, max);
        return dis(RNG::getInstance().gen);
    }
    static float uniform(float const min, float const max) {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(RNG::getInstance().gen);
    }
    static int uniform(int const min, int const max) {
        std::uniform_int_distribution<int> dis(min, max);
        return dis(RNG::getInstance().gen);
    }
    static size_t uniform(size_t const max) {
        std::uniform_int_distribution<size_t> dis(0, max);
        return dis(RNG::getInstance().gen);
    }


private:
    // Private constructor to prevent instantiation
    RNG() : gen(std::random_device()()) {}

    // Mersenne Twister pseudo-random number generator
    std::mt19937 gen;
};

//static RNG random = RNG::getInstance();

#endif