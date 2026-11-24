#include "Nilib/ML/CNodes/Layer/MeanScale.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

MeanScale::MeanScale(CNode &X) : d_X(X), d_mu(X.value.rows(), X.value.cols()), d_var(X.value.rows(), X.value.cols())
{
    d_mu.zero();
    d_var.zero();
}

void MeanScale::evaluate()
{
    d_X.evaluate();

    // Update running average.
    d_mu = 0.99f * d_mu + 0.01f * d_X.value;
    // Update running variance.

    // Apply transformation.
    this->value = d_X.value - d_mu;
}

void MeanScale::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == this->value.rows());
    CORE_ASSERT(seed.cols() == this->value.cols());
    d_X.derive(seed);
}