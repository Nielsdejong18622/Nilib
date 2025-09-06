#ifndef _RANDOM_OPTIMIZER_HPP
#define _RANDOM_OPTIMIZER_HPP

#include "Nilib/ML/Optimizers/Optimizer.hpp"

namespace Nilib
{

    struct Random : public Optimizer
    {

        Random(std::vector<Weight *> weights)
            : Optimizer(weights)
        {
        }

        void updateGrad() const override
        {
            ASSERT(checkgradients(), "One or more gradients are not yet calculated!");
            for (Weight *w : weights)
            {
                w->value += Nilib::Matrixf::randn(w->value.rows(), w->value.cols(), 0.0f, 0.01f);
            }
        }
    };

} // namespace Nilib

#endif