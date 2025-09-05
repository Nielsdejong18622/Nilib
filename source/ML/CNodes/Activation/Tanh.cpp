#include "Nilib/ML/CNodes/Activation/Tanh.h"

using namespace Nilib;

float Tanh_fun::evaluate(float const t)
{
    return std::tanh(t);
}
float Tanh_fun::derivative(float const t)
{
    float tmp = std::cosh(t);
    return 1.0f / (tmp * tmp);
}