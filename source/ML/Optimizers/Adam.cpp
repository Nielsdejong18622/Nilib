#include "Nilib/ML/Optimizers/Adam.hpp"

#include "Nilib/Math/LinALg.hpp"

using namespace Nilib;

Nilib::Adam::Adam(Module *module, float learning_rate, float weight_decay)
    : Optimizer(module), d_lr(learning_rate), d_weight_decay(weight_decay), d_step(0)
{
}

Nilib::Adam::Adam(Weightptrs const &weights, float learning_rate, float weight_decay)
    : Optimizer(weights), d_lr(learning_rate), d_weight_decay(weight_decay), d_step(0)
{
}

void Nilib::Adam::updateGrad(float const multi)
{
    float const beta = 0.9f;    // Momentum
    float const beta2 = 0.999f; // Velocity
    float const epsilon = 1e-8f;
    if (d_step == 0)
        init();

    ++d_step;
    float const bias_correct_m = 1.0f - std::pow(beta, d_step);
    float const bias_correct_v = 1.0f - std::pow(beta2, d_step);

    for (size_t w_idx = 0; w_idx < weights.size(); w_idx++)
    {
        Nilib::Matrixf &grad = weights[w_idx]->partial;

        // Apply L2 weight decay directly.
        grad += d_weight_decay * weights[w_idx]->value;

        // Update momentum (1st moment)
        d_momentum[w_idx] = beta * d_momentum[w_idx] + (1.0f - beta) * grad;

        // Update velocity (2nd moment)
        auto const square = [](float t)
        { return t * t; };
        d_velocity[w_idx] = beta2 * d_velocity[w_idx] + (1.0f - beta2) * Nilib::apply(grad, square);

        // Bias-corrected moments
        Nilib::Matrixf m_hat = d_momentum[w_idx] / bias_correct_m;
        Nilib::Matrixf v_hat = d_velocity[w_idx] / bias_correct_v;

        // Final update
        for (size_t i = 0; i < weights[w_idx]->value.size(); ++i)
        {
            float denom = std::sqrt(v_hat(i)) + epsilon;
            weights[w_idx]->value(i) -= d_lr * multi * m_hat(i) / denom;
        }
    }

    // size_t w_idx = 3;
    // LOG_DEBUG("Adam weight", w_idx,
    //           "value:", weights[w_idx]->value.avg(),
    //           "partial:", weights[w_idx]->partial.avg(),
    //           "momentum:", d_momentum[w_idx].avg(),
    //           "velocity:", d_velocity[w_idx].avg());
}

void Nilib::Adam::init()
{
    ASSERT(checkgradients(), "Atleast one forward pass required for Adam!");
    // LOG_DEBUG("Initializing Adam for", weights.size(), "weights");
    for (auto *w : weights)
    {
        CORE_ASSERT(w != nullptr);
        CORE_ASSERT(w->value.size() > 0);
    }
    for (Weight const *weight : weights)
    {
        d_momentum.push_back(Nilib::Matrixf::zeros(weight->value.rows(), weight->value.cols()));
        d_velocity.push_back(Nilib::Matrixf::zeros(weight->value.rows(), weight->value.cols()));
    }
}
//         void updateGradSGD(float const multi, float const learning_rate)
//         {

//             // Store the momentum in the grad_values values and the velocity in the partials.
//             float const alpha = multi * learning_rate;
//             for (size_t w_idx = 0; w_idx < weights.size(); w_idx++)
//             {
//                 // Step.
//                 weights[w_idx].value += alpha * weights[w_idx].partial;
//             }
//         }
