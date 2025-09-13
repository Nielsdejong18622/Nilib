#ifndef _CNODE_SIGMOID_HPP
#define _CNODE_SIGMOID_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

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