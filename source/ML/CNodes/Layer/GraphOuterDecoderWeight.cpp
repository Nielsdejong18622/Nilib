#include "Nilib/ML/CNodes/Layer/GraphOuterDecoderWeight.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

OuterProductDecoderWeight::OuterProductDecoderWeight(CNode &X, size_t xcol)
    : X(X), W(xcol, xcol)
{
    // LOG_DEBUG("Constructed Outerproductdecoder!");
}

void OuterProductDecoderWeight::evaluate()
{
    // LOG_DEBUG("OuterProductDecoder eval!");
    X.evaluate();
    // // float scalefactor = X.value.rows();
    this->value = X.value * W.value * transpose(X.value);
    // this->value /= scalefactor;
}

void OuterProductDecoderWeight::derive(Nilib::Matrixf const &seed)
{
    W.derive(Nilib::transpose(X.value) * seed * X.value);
    X.derive(seed * X.value * Nilib::transpose(W.value) + Nilib::transpose(seed) * X.value * W.value);
}

void OuterProductDecoderWeight::learnables(Nilib::Module::Weights &add)
{
    add.push_back(&W);
}
