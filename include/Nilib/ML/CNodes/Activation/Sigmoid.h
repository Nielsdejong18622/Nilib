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

    struct SigmoidLinear : public Activation
    {
        SigmoidLinear(CNode *input)
            : Activation(input,
                         std::bind(&SigmoidLinear::sigmoidlinear, this, std::placeholders::_1),
                         std::bind(&SigmoidLinear::sigmoidlinear_deriv, this, std::placeholders::_1)) {}

        float sigmoidlinear(float const t)
        {
            if (t > 10.0f)
                return t;
            if (t < -6.0f)
                return -0.0f;
            return t / (1.0f + std::exp(-1.0f * t));
        }

        float sigmoidlinear_deriv(float const t)
        {
            if (t > 10.0f)
                return 1.0f;
            if (t < -6.0f)
                return 0.0;
            float const e_t = std::exp(-1.0f * t);
            float const tmp = 1.0f + e_t;
            return (tmp + t * e_t) / (tmp * tmp);
        }
    };
}
#endif