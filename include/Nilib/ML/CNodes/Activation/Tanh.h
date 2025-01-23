
#ifndef _CNODE_TANH_H
#define _CNODE_TANH_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Tanh : public Activation
    {
        float multiplier;
        Tanh(CNode *input, float const multiplier)
            : Tanh(input) { this->multiplier = multiplier;}

        Tanh(CNode *input)
            : Activation(input,
                         std::bind(&Tanh::tanh, this, std::placeholders::_1),
                         std::bind(&Tanh::tanh_deriv, this, std::placeholders::_1)), multiplier(1.0f) {}

        float tanh(float const t) { return multiplier * std::tanh(t); }
        float tanh_deriv(float const t)
        {
            float tmp = std::tanh(t);
            return multiplier - multiplier * tmp * tmp;
        }
    };

}
#endif