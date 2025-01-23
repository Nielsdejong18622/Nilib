
#ifndef _CNODE_RBF_H
#define _CNODE_RBF_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct RBF : public Activation
    {
        float d_multi = 1.0f;
        RBF(CNode *input, float const multi)
            : RBF(input) { d_multi = multi;}
        RBF(CNode *input)
            : Activation(input,
                         std::bind(&RBF::rbf, this, std::placeholders::_1),
                         std::bind(&RBF::rbf_deriv, this, std::placeholders::_1)) {}

        float rbf(float const t) { return std::exp(-d_multi * t * t); }
        float rbf_deriv(float const t) { return -d_multi * t * std::exp(-d_multi * t * t); }
    };

}
#endif