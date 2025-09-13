#ifndef _ML_MLP_H
#define _ML_MLP_H

#include "Nilib/ML/Models/Model.hpp"

namespace Nilib
{
    /// @brief Takes a row vector x [1 * col] and multiplies by W [col * neurons], adds b [1, neurons].
    class MultilayerPerceptron : public Module
    {
    public:
        MultilayerPerceptron(CNode &x, size_t const x_col, size_t const neurons);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;

        void learnables(Weights &add) override;

    protected:
        CNode &x;
        Weight W;
        Weight b;
    };

} // namespace Nilib

#endif