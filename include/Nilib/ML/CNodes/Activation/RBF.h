
#ifndef _CNODE_RBF_H
#define _CNODE_RBF_H

#include "Nilib/ML/CNodes/Activation/Activation.h"

namespace Nilib
{

    struct RBF_fun
    {
        static float evaluate(float const t);
        static float derivative(float const t);
    };

    using RBF = Activation<RBF_fun>;

}
#endif