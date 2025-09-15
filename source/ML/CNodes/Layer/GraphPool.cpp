#include "Nilib/ML/CNodes/Layer/GraphPool.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GraphPoolAverage::GraphPoolAverage(CNode &X)
    : X(X)
{
}

void GraphPoolAverage::evaluate()
{
    X.evaluate();
    CORE_ASSERT(X.value.rows() > 0);
    CORE_ASSERT(X.value.cols() > 0);
    d_cached_rows = X.value.rows();
    this->value = Nilib::colMeans(X.value);
}

void GraphPoolAverage::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(d_cached_rows > 0.0);
    CORE_ASSERT(seed.rows() == 1);
    Matrixf iota = Matrixf::ones(d_cached_rows, 1); // Already transposed.
    X.derive(iota * seed / d_cached_rows);
}