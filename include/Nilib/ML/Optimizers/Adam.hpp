#ifndef _ADAM_OPTIMIZER_HPP
#define _ADAM_OPTIMZIER_HPP

#include "Nilib/ML/Optimizers/Optimizer.hpp"

namespace Nilib
{

    class Adam : public Optimizer
    {
        // Make a copy of the weights for the momentum and variance of each parameter.
        std::vector<Weight> momentum;
        std::vector<Weight> variance;

    public:
        void updateGrad() const override;
    };
} // namespace Nilib

#endif