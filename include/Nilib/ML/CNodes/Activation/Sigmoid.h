#ifndef _CNODE_SIGMOID_H
#define _CNODE_SIGMOID_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Sigmoid_fun
    {
        static float evaluate(float const t) { return 1.0f / (1.0f + std::exp(-1.0f * t)); }

        static float derivative(float const t)
        {
            float tmp = evaluate(t);
            return tmp * (1.0f - tmp);
        }
    };

    using Sigmoid = Activation<Sigmoid_fun>;
}
#endif