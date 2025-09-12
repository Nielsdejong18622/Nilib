
#ifndef _CNODE_RELU_H
#define _CNODE_RELU_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{
    template <float Leaky = 0.0f>
    struct LeakyRelu_fun
    {
        static float evaluate(float x) { return x > 0 ? x : Leaky * x; }
        static float derivative(float x) { return x > 0 ? 1.0f : Leaky; }
    };

    using Relu = Activation<LeakyRelu_fun<0.0f>>;
    using LeakyRelu = Activation<LeakyRelu_fun<0.001f>>;

    template <float Leaky = 0.0f>
    struct NegLeakyRelu_fun
    {
        static float evaluate(float const t) { return (t < 0) ? t : Leaky * t; }
        static float derivative(float const t) { return (t < 0) ? 1 : Leaky; }
    };

    using NegRelu = Activation<NegLeakyRelu_fun<0.0f>>;

}
#endif