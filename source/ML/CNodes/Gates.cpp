#include "Nilib/ML/CNodes/Gates.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

Minus::Minus(CNode &X, CNode &Y)
    : X(X), Y(Y)
{
}

void Minus::evaluate()
{
    X.evaluate();
    Y.evaluate();
    this->value = X.value - Y.value;
}

void Minus::derive(Matrixf const &seed)
{
    X.derive(seed);
    Y.derive(-1.0f * seed);
}

Plus::Plus(CNode &X, CNode &Y)
    : X(X), Y(Y)
{
}

void Plus::evaluate()
{
    X.evaluate();
    Y.evaluate();
    this->value = X.value + Y.value;
}

void Plus::derive(Matrixf const &seed)
{
    X.derive(seed);
    Y.derive(seed);
}
