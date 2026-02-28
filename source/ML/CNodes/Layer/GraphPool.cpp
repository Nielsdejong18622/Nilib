#include "Nilib/ML/CNodes/Layer/GraphPool.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GraphPoolAverage::GraphPoolAverage(CNode &X) : d_X(X)
{
}

void GraphPoolAverage::evaluate()
{
    d_X.evaluate();
    CORE_ASSERT(d_X.value.rows() > 0);
    CORE_ASSERT(d_X.value.cols() > 0);
    d_cached_rows = d_X.value.rows();
    this->value = Nilib::colMeans(d_X.value);
}

void GraphPoolAverage::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(d_cached_rows > 0);
    CORE_ASSERT(seed.rows() == 1);
    Matrixf iota = Matrixf::ones(d_cached_rows, 1); // Already transposed.
    Matrixf tmp = iota * seed / static_cast<float>(d_cached_rows);

    CORE_ASSERT(tmp.rows() == d_X.value.rows());
    CORE_ASSERT(tmp.cols() == d_X.value.cols());
    d_X.derive(tmp);
}