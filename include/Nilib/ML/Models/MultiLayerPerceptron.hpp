#ifndef _ML_MLP_H
#define _ML_MLP_H

#include "Nilib/Math/LinALg.hpp"

#include "Nilib/ML/CNodes/Weight.h"

namespace Nilib
{
    /// @brief Takes a row vector x [1 * col] and multiplies by W [col * neurons], adds b [1, neurons].
    struct MultilayerPerceptron : public Input
    {
        CNode *x = nullptr;
        Weight W;
        Weight b;

        MultilayerPerceptron(CNode *x, size_t x_col, size_t neurons);
        void evaluate() override;

        void derive(Nilib::Matrixf const &seed) override;
    };

} // namespace Nilib

#endif