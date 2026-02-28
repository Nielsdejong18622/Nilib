#include "Nilib/ML/CNodes/Layer/GraphOuterDecoder.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

OuterProductDecoder::OuterProductDecoder(CNode &X) : d_X(X)
{
    // LOG_DEBUG("Constructed Outerproductdecoder!");
}

void OuterProductDecoder::evaluate()
{
    // LOG_DEBUG("OuterProductDecoder eval!");
    d_X.evaluate();
    // // float scalefactor = X.value.rows();
    this->value = d_X.value * transpose(d_X.value);
    // this->value /= scalefactor;
}

void OuterProductDecoder::derive(Nilib::Matrixf const &seed)
{
    // LOG_DEBUG("OuterProductDecoder derive!");
    // CORE_ASSERT(seed.cols() == X.value.rows());
    // CORE_ASSERT(X.value.rows() == seed.rows());
    // // float scalefactor = X.value.rows();
    d_X.derive(seed * d_X.value + Nilib::transpose(seed) * d_X.value);
}
