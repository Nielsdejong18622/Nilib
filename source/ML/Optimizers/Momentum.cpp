#include "Nilib/ML/Optimizers/Momentum.hpp"
#include "Nilib/ML/CNodes/Weight.h"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

Nilib::Momentum::Momentum(Weightptrs const &weights, float learning_rate, float weight_decay)
    : Optimizer(weights), d_lr(learning_rate), d_weight_decay(weight_decay), d_step(0)
{
    ASSERT(weight_decay != 0.0f, "Not implemented yet!");
}

void Nilib::Momentum::updateGrad(float const multi)
{
    float const beta = 0.9f; // Momentum

    if (d_step == 0)
        init();

    ++d_step;
    float const bias_correct_m = 1.0f - std::pow(beta, d_step);

    for (size_t w_idx = 0; w_idx < weights.size(); w_idx++)
    {
        Nilib::Matrixf const &grad = weights[w_idx]->partial;

        // Update momentum (1st moment)
        d_momentum[w_idx] = beta * d_momentum[w_idx] + (1.0f - beta) * grad;

        // Final update
        weights[w_idx]->value -= multi * d_lr * d_momentum[w_idx] / bias_correct_m;
    }
}

void Nilib::Momentum::init()
{
    ASSERT(checkgradients(), "Atleast one forward pass required for Momentum!");
    for (auto *w : weights)
    {
        CORE_ASSERT(w != nullptr);
        CORE_ASSERT(w->value.size() > 0);
    }
    for (Weight const *weight : weights)
    {
        d_momentum.push_back(Nilib::Matrixf::zeros(weight->value.rows(), weight->value.cols()));
    }
}
