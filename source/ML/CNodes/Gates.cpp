#include "Nilib/ML/CNodes/Gates.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

Minus::Minus(CNode &X, CNode &Y) : d_X(X), d_Y(Y)
{
}

void Minus::evaluate()
{
    d_X.evaluate();
    d_Y.evaluate();
    this->value = d_X.value - d_Y.value;
}

void Minus::derive(Matrixf const &seed)
{
    d_X.derive(seed);
    d_Y.derive(-1.0f * seed);
}

Plus::Plus(CNode &X, CNode &Y) : d_X(X), d_Y(Y)
{
}

void Plus::evaluate()
{
    d_X.evaluate();
    d_Y.evaluate();
    this->value = d_X.value + d_Y.value;
}

void Plus::derive(Matrixf const &seed)
{
    d_X.derive(seed);
    d_Y.derive(seed);
}
