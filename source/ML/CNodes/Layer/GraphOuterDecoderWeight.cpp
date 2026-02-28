#include "Nilib/ML/CNodes/Layer/GraphOuterDecoderWeight.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

OuterProductDecoderWeight::OuterProductDecoderWeight(CNode &X, size_t xcol) : d_X(X), d_W(xcol, xcol)
{
    // LOG_DEBUG("Constructed Outerproductdecoder!");
}

void OuterProductDecoderWeight::evaluate()
{
    // LOG_DEBUG("OuterProductDecoder eval!");
    d_X.evaluate();
    // // float scalefactor = X.value.rows();
    this->value = d_X.value * d_W.value * transpose(d_X.value);
    // this->value /= scalefactor;
}

void OuterProductDecoderWeight::derive(Nilib::Matrixf const &seed)
{
    d_W.derive(Nilib::transpose(d_X.value) * seed * d_X.value);
    d_X.derive(seed * d_X.value * Nilib::transpose(d_W.value) + Nilib::transpose(seed) * d_X.value * d_W.value);
}

void OuterProductDecoderWeight::learnables(Nilib::Module::Weights &add)
{
    add.push_back(&d_W);
}
