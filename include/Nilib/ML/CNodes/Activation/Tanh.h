
#ifndef _CNODE_TANH_H
#define _CNODE_TANH_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Tanh_fun
    {
        static float evaluate(float const t);
        static float derivative(float const t);
    };

    using Tanh = Activation<Tanh_fun>;
}
#endif