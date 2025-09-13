#include "Nilib/ML/CNodes/Activation/RBF.hpp"

using namespace Nilib;

float RBF_fun::evaluate(float const t)
{
    return std::exp(t * t);
}

float RBF_fun::derivative(float const t)
{
    return -1.0 * t * std::exp(-1.0 * t * t);
}
