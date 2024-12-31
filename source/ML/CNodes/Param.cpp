#include "Nilib/ML/CNodes/Param.h"

namespace Nilib {
    
    Param::Param(float value)
    : Input(Matrixf(1,1,{value}))
    {
    }

}