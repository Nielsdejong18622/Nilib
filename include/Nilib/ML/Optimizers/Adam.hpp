#ifndef _ADAM_OPTIMIZER_HPP
#define _ADAM_OPTIMZIER_HPP

#include "Nilib/ML/Optimizers/Optimizer.hpp"

namespace Nilib
{

    class Adam : public Optimizer
    {
    public:
        /// @brief Updates gradients according to the Adam optimizer.
        /// @param weights # Pointer of weights to optimize.
        /// @param learning_rate Learning rate.
        /// @param weight_decay Squared norm.
        Adam(Weightptrs const &weights, float learning_rate = 0.001f, float weight_decay = 0.0f);
        void updateGrad(float const multi = 1.0f) override;

    protected:
        void init();

        std::vector<Matrixf> d_momentum; // Store momentum in momentum.value
        std::vector<Matrixf> d_velocity; // Store momentum in momentum.value
        float d_lr;                      // Learning rate.
        float d_weight_decay;
        size_t d_step; // How often updateGrad has been called.
    };
} // namespace Nilib

#endif