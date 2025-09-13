#include "Nilib/ML/CNodes/Param.hpp"

namespace Nilib
{

    Param::Param(float const value)
        : GNode(Matrixf(1, 1, {value}))
    {
    }

}