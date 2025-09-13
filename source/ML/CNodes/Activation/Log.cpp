#include "Nilib/ML/CNodes/Activation/Log.hpp"

using namespace Nilib;

float Log_fun::evaluate(float const t)
{
    return std::log(std::clamp(t, 10e-8f, 1.0f - 10e-8f));
}

float Log_fun::derivative(float const t)
{
    return 1.0f / (t);
}
