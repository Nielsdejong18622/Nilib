#ifndef _RNG_H
#define _RNG_H

#include <random>
#include <algorithm>

class RNG {

public:
    // Get the singleton instance
    static RNG& getInstance() {
        static RNG instance;
        return instance;
    }

    template<typename T>
    static T const &sample(std::vector<T> const &population) {
        return population[RNG::uniform<size_t>(0, population.size() - 1)];
    }

    static void seed(size_t const seed) {
        RNG::getInstance().gen = std::mt19937(seed);
    }
    
    // Generate a random probability between 0 and 1
    template<typename T = float>
    static T prob() {
        static_assert(std::is_arithmetic<T>::value, 
                      "result_type must be a number type");
        std::uniform_real_distribution<T> dis(0.0, 1.0);
        return dis(RNG::getInstance().gen);
    }


    // Generate a uniform value in [min, max]
    template<typename T = double>
    static T uniform(T const min, T const max) {
        if constexpr (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> dis(min, max);
            return dis(RNG::getInstance().gen);
        } else if constexpr (std::is_floating_point_v<T>){
            std::uniform_real_distribution<T> dis(min, max);
            return dis(RNG::getInstance().gen);
        } else {
            static_assert(false, "Type must be integral or float-like value.");
        }
        }
    // Generate a lognormal value. 
    template<typename T = double>
    static T lognormal(T const mean, T const stddev) {
        static_assert(std::is_arithmetic<T>::value, 
                      "result_type must be a number type");
        std::lognormal_distribution<T> dis(mean, stddev);
        return dis(RNG::getInstance().gen);
    }
    
    // Generate a random normal variable. 
    template<typename T = double>
    static T normal(T const mean, T const stddev) {
        static_assert(std::is_arithmetic<T>::value, 
                      "result_type must be a number type");
        std::normal_distribution<T> dis(mean, stddev);
        return dis(RNG::getInstance().gen);
    }
    
    // Generate a random weibull variable. 
    template<typename T = double>
    static T weibull(T const shape, T const scale) {
        static_assert(std::is_arithmetic<T>::value, 
                      "result_type must be a number type");
        std::weibull_distribution<T> dis(shape, scale);
        return dis(RNG::getInstance().gen);
    }

    // Shuffle a container(vector) in place. 
    template<typename container>
    static void shuffle(container &cont)
    {
        std::ranges::shuffle(std::begin(cont), std::end(cont), RNG::getInstance().gen);
    }

private:
    // Private constructor to prevent instantiation
    RNG() : gen(std::random_device()()) {}

    // Mersenne Twister pseudo-random number generator
    std::mt19937 gen{std::random_device()()};
};

//static RNG random = RNG::getInstance();

#endif