#include "Nilib/ML/CNodes/Param.hpp"

namespace Nilib
{

    Param::Param(float val)
        : GNode(Matrixf(1, 1, {val}))
    {
    }

}