
#ifndef _CNODE_SELU_HPP
#define _CNODE_SELU_HPP

#include "Nilib/ML/CNodes/Activation/Activation.hpp"

namespace Nilib
{
struct Selu_fun
{
    static constexpr float lambda = 1.05070f;
    static constexpr float alpha = 1.67326f;
    static float evaluate(float x)
    {
        return x > 0 ? lambda * x : lambda * alpha * (std::exp(x) - 1.0f);
    }
    static float derivative(float x)
    {
        return x > 0 ? lambda : lambda * alpha * std::exp(x);
    }
};

using Selu = Activation<Selu_fun>;

} // namespace Nilib
#endif