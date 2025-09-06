#include "Nilib/ML/Optimizers/Adam.hpp"

using namespace Nilib;

// Nilib::Adam::Adam(std::vector<Weight *> weights)
//     : Optimizer(weights)
// {
// }

// Overwrite
void Nilib::Adam::updateGrad() const
{
    //         // Gradient Descent.
    //         void updateGrad(float const multi, float const learning_rate, float const step)
    //         {
    //             float const beta = 0.9f;    // Momentum
    //             float const beta2 = 0.999f; // Velocity
    //             float const bias_correct_m = (1 - std::pow(beta, step + 1));
    //             float const bias_correct_v = (1 - std::pow(beta2, step + 1));
    //             float const weight_decay = 0.01f;

    //             // Store the momentum in the grad_values values and the velocity in the partials.
    //             float const epsilon = 1e-8;
    //             for (size_t w_idx = 0; w_idx < grad_weigths.size(); w_idx++)
    //             {
    //                 // Grad
    //                 auto grad = weights[w_idx].partial; //- weight_decay * weights[w_idx].value;

    //                 // Update the momentum.
    //                 grad_weigths[w_idx].value = beta * grad_weigths[w_idx].value + (1 - beta) * grad;

    //                 // Update the velocity.
    //                 grad_weigths[w_idx].partial = beta2 * grad_weigths[w_idx].partial + (1.0 - beta2) * Nilib::apply(grad, [](float const t)
    //                                                                                                                  { return t * t; });

    //                 // Step.
    //                 for (int i = 0; i < weights[w_idx].value.size(); ++i)
    //                     weights[w_idx].value(i) += learning_rate * multi * grad_weigths[w_idx].value(i) / (bias_correct_m * (std::sqrt(grad_weigths[w_idx].partial(i) / bias_correct_v) + epsilon));
    //             }

    //             // if (A_W5.partial.sum() == 0.0)
    //             // LOG_DEBUG("Updating Gradient", this, avgWeight(), avgGradient(), "Adam", avgAdamMomentum(), avgAdamVelocity());
    //             // LOG_WARNING("Critic Update multi", multi, "Wval", avgWeight(), "MomentumSum", grad_weigths[0].value.sum(), "veloc", grad_weigths[0].value.sum(), "partialsum", weights[0].partial.sum(), bias_correct_m);
    //         }
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
}