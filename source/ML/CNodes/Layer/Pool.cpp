#include "Nilib/ML/CNodes/Layer/Pool.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

MeanPool::MeanPool(CNode &X) : d_X(X), d_row(X.value.rows()), d_col(X.value.cols())
{
}

void MeanPool::evaluate()
{
    d_X.evaluate();
    d_row = d_X.value.rows();
    d_col = d_X.value.cols();
    CORE_ASSERT(d_X.value.size() > 0);

    this->value = Nilib::Matrixf(1, 1, d_X.value.avg());
}

void MeanPool::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(d_row * d_col > 0);
    CORE_ASSERT(seed.rows() == 1);
    CORE_ASSERT(seed.cols() == 1);
    d_X.derive(Nilib::Matrixf::ones(d_row, d_col) * seed / static_cast<float>(d_row * d_col));
}