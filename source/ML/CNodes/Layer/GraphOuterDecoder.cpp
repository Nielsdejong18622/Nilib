#include "Nilib/ML/CNodes/Layer/GraphOuterDecoder.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

OuterProductDecoder::OuterProductDecoder(CNode &X)
    : X(X)
{
    // LOG_DEBUG("Constructed Outerproductdecoder!");
}

void OuterProductDecoder::evaluate()
{
    // LOG_DEBUG("OuterProductDecoder eval!");
    X.evaluate();
    // // float scalefactor = X.value.rows();
    this->value = X.value * transpose(X.value);
    // this->value /= scalefactor;
}

void OuterProductDecoder::derive(Nilib::Matrixf const &seed)
{
    // LOG_DEBUG("OuterProductDecoder derive!");
    // CORE_ASSERT(seed.cols() == X.value.rows());
    // CORE_ASSERT(X.value.rows() == seed.rows());
    // // float scalefactor = X.value.rows();
    X.derive(seed * X.value + Nilib::transpose(seed) * X.value);
}
