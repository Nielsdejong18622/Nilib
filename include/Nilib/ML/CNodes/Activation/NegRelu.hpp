
#ifndef _CNODE_NEGRELU_HPP
#define _CNODE_NEGRELU_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

namespace Nilib
{
    template <float Leaky = 0.0f>
    struct NegLeakyRelu_fun
    {
        static float evaluate(float const t) { return (t < 0) ? t : Leaky * t; }
        static float derivative(float const t) { return (t < 0) ? 1 : Leaky; }
    };

    using NegRelu = Activation<NegLeakyRelu_fun<0.0001f>>;

}
#endif