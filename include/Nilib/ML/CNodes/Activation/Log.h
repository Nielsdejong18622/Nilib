
#ifndef _CNODE_LOG_H
#define _CNODE_LOG_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Log : public Activation
    {
        Log(CNode *input)
            : Activation(input,
                         std::bind(&Log::log, this, std::placeholders::_1),
                         std::bind(&Log::log_deriv, this, std::placeholders::_1)) {}

        float log(float const t) { return std::log(t); }
        float log_deriv(float const t)
        {
            return 1.0f / t;
        }
    };

}
#endif