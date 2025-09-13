#include "Nilib/ML/CNodes/Layer/Pool.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

MeanPool::MeanPool(CNode &X)
    : X(X), d_row(X.value.rows()), d_col(X.value.cols())
{
}

void MeanPool::evaluate()
{
    X.evaluate();
    d_row = X.value.rows();
    d_col = X.value.cols();
    CORE_ASSERT(X.value.size() > 0);

    this->value = Nilib::Matrixf(1, 1, {X.value.avg()});
}

void MeanPool::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(d_row * d_col > 0);
    CORE_ASSERT(seed.rows() == 1);
    CORE_ASSERT(seed.cols() == 1);
    X.derive(Nilib::Matrixf::ones(d_row, d_col) * seed / static_cast<float>(d_row * d_col));
}