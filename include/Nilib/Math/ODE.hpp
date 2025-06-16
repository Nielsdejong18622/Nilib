#ifndef _ODE_HPP
#define _ODE_HPP

#include "Nilib/Math/Matrix.hpp"

namespace Nilib
{
    
    // Standard ODE solver framework.
    
    // A differential function. 
    // y' = f(y)
    Nilib::Matrixf ODEfunction(Nilib::Matrixf const &X);

} // namespace Nilib

#endif