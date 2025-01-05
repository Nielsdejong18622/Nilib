#ifndef _CNODE_SIGMOID_H
#define _CNODE_SIGMOID_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Sigmoid : public Activation
    {
        Sigmoid(CNode *input)
            : Activation(input,
                         std::bind(&Sigmoid::sigmoid, this, std::placeholders::_1),
                         std::bind(&Sigmoid::sigmoid_deriv, this, std::placeholders::_1)) {}

        float sigmoid(float const t) { return 1.0f / (1.0f + std::exp(-1.0f * t)); }

        float sigmoid_deriv(float const t)
        {
            float tmp = sigmoid(t);
            return tmp * (1.0f - tmp);
        }
    };
}
#endif