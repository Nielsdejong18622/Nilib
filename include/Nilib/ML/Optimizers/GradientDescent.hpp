#ifndef _MOMENTUM_OPTIMIZER_HPP
#define _MOMENTUM_OPTIMZIER_HPP

#include "Nilib/ML/Optimizers/Optimizer.hpp"

namespace Nilib
{

    class GradientDescent : public Optimizer
    {
    public:
        /// @brief Updates gradients according to the GradientDescent optimizer.
        /// @param weights # Pointer of weights to optimize.
        /// @param learning_rate Learning rate.
        /// @param weight_decay Squared norm.
        GradientDescent(Weightptrs const &weights, float learning_rate = 0.001f, float weight_decay = 0.0f);
        void updateGrad(float const multi = 1.0f) override;

    protected:

        float d_lr;                      // Learning rate.
        float d_weight_decay;            // L2 weight decay
    };
} // namespace Nilib

#endif