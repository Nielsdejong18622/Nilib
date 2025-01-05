
#ifndef _CNODE_RELU_H
#define _CNODE_RELU_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{
    struct Relu : public Activation
    {
        float d_leaky;
        Relu(CNode *input, float leaky)
            : d_leaky(leaky), Activation(input,
                                         std::bind(&Relu::relu, this, std::placeholders::_1),
                                         std::bind(&Relu::relu_deriv, this, std::placeholders::_1)) {}

        float relu(float const t)
        {
            return (t > 0) ? t : d_leaky * t;
        }
        float relu_deriv(float const t)
        {
            return (t > 0) ? 1 : d_leaky;
        }
    };
}
#endif