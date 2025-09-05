#include "Nilib/ML/CNodes/Activation/Sigmoid.h"

using namespace Nilib;

float Sigmoid_fun::evaluate(float const t)
{
    return 1.0f / (1.0f + std::exp(-1.0f * t));
}

float Sigmoid_fun::derivative(float const t)
{
    float tmp = evaluate(t);
    return tmp * (1.0f - tmp);
}