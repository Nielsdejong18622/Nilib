
#ifndef _CNODE_TANH_HPP
#define _CNODE_TANH_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

namespace Nilib
{

    struct Tanh_fun
    {
        static float evaluate(float const t);
        static float derivative(float const t);
    };

    template <float multiplier = 1.0f>
    struct Tanh_fun_scaled
    {
        static float evaluate(float const t)
        {
            return multiplier * std::tanh(t / multiplier);
        }

        static float derivative(float const t)
        {
            float tmp = std::cosh(t / multiplier);
            return 1.0f / (tmp * tmp);
        }
    };

    using Tanh = Activation<Tanh_fun>;
}
#endif