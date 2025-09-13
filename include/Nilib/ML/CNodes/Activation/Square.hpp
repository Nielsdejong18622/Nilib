
#ifndef _CNODE_SQUARE_HPP
#define _CNODE_SQUARE_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

namespace Nilib
{

    struct Square_fun
    {
        static float evaluate(float const t);
        static float derivative(float const t);
    };

    using Square = Activation<Square_fun>;

}
#endif