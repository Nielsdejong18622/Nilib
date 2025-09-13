
#include "Nilib/ML/CNodes/Layer/GraphConv.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GraphConv::GraphConv(CNode &A, CNode &X, size_t const colX, size_t const outdim)
    : A(A), X(X), W(colX, outdim)
{
    CORE_ASSERT(outdim > 0);
    CORE_ASSERT(colX > 0);
}

void GraphConv::evaluate()
{
    A.evaluate();
    X.evaluate();
    W.evaluate();
    ASSERT(A.value.cols() == X.value.rows(), "AX undefined:", A.value, 'x', X.value);
    ASSERT(X.value.cols() == W.value.rows(), "XW undefined:", X.value, 'x', W.value);
    this->value = A.value * X.value * W.value;
}

void GraphConv::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == A.value.rows());
    CORE_ASSERT(seed.cols() == W.value.cols());
    A.derive(seed * transpose(X.value * W.value));
    X.derive(transpose(A.value) * seed * transpose(W.value));
    W.derive(transpose(A.value * X.value) * seed);
}