
#ifndef _CNODE_LINEAR_HPP
#define _CNODE_LINEAR_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

namespace Nilib
{
    struct Linear_fun
    {
        static float evaluate(float x) { return x; }
        static float derivative(float x) { return 1.0; }
    };

    using Linear = Activation<Linear_fun>;

}
#endif