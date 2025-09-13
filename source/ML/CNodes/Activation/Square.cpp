#include "Nilib/ML/CNodes/Activation/Square.hpp"

using namespace Nilib;

float Square_fun::evaluate(float const t)
{
    return 0.5 * t * t;
}

float Square_fun::derivative(float const t)
{
    return t;
}
