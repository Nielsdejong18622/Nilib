#ifndef _CNODE_SIGMOID_H
#define _CNODE_SIGMOID_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct Sigmoid_fun
    {
        static float evaluate(float const t);
        static float derivative(float const t);
    };

    using Sigmoid = Activation<Sigmoid_fun>;
}
#endif