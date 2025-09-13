
#ifndef _CNODE_LOG_HPP
#define _CNODE_LOG_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

namespace Nilib
{

    struct Log_fun
    {
        static float evaluate(float const t);
        static float derivative(float const t);
    };

}
#endif