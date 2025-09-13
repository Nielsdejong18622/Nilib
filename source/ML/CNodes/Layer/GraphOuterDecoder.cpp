#include "Nilib/ML/CNodes/Layer/GraphOuterDecoder.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

OuterProductDecoder::OuterProductDecoder(CNode &X)
    : X(X)
{
}

void OuterProductDecoder::evaluate()
{
    X.evaluate();
    float scalefactor = X.value.rows();
    this->value = X.value * transpose(X.value);
    this->value /= scalefactor;
}

void OuterProductDecoder::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.cols() == X.value.rows());
    CORE_ASSERT(X.value.rows() == seed.rows());
    float scalefactor = X.value.rows();
    X.derive(seed * X.value / scalefactor + Nilib::transpose(Nilib::transpose(X.value) * seed / scalefactor));
}
