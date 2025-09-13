
#ifndef _CNODE_RBF_HPP
#define _CNODE_RBF_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

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