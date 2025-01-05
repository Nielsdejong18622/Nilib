
#ifndef _CNODE_TANH_H
#define _CNODE_TANH_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Tanh : public Activation
    {
        Tanh(CNode *input)
            : Activation(input,
                         std::bind(&Tanh::tanh, this, std::placeholders::_1),
                         std::bind(&Tanh::tanh_deriv, this, std::placeholders::_1)) {}

        float tanh(float const t) { return std::tanh(t); }
        float tanh_deriv(float const t)
        {
            float tmp = std::tanh(t);
            return 1 - tmp * tmp;
        }
    };

}
#endif