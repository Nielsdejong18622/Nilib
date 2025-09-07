#include "Nilib/ML/Optimizers/GradientDescent.hpp"
#include "Nilib/ML/CNodes/Weight.h"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

Nilib::GradientDescent::GradientDescent(Weightptrs const &weights, float learning_rate, float weight_decay)
    : Optimizer(weights), d_lr(learning_rate), d_weight_decay(weight_decay)
{
    ASSERT(weight_decay != 0.0f, "Not implemented yet!");
}

void Nilib::GradientDescent::updateGrad(float const multi)
{
    for (size_t w_idx = 0; w_idx < weights.size(); w_idx++)
    {
        weights[w_idx]->value -= multi * d_lr * weights[w_idx]->partial;
    }
}
