#include "Nilib/ML/CNodes/Layer/Norm.h"
#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

Norm::Norm(CNode *X, size_t xrow, size_t xcol)
{
    mean = Nilib::Matrixf::zeros(xrow, xcol);
}

void Norm::evaluate()
{
    CORE_ASSERT(x);
    x->evaluate();

    mean = 0.99f * x->value + 0.01f * x->value;

    this->value = (x->value - mean);
}

void Norm::derive(Nilib::Matrixf const &seed)
{
    x->derive(seed);
}
