#include "Nilib/ML/CNodes/Layer/GraphConvHighWay.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GraphConvHigh::GraphConvHigh(CNode &A, CNode &X, size_t const colX, size_t const outdim)
    : A(A), X(X), W1(colX, outdim), W2(colX, outdim)
{
    CORE_ASSERT(outdim > 0);
    CORE_ASSERT(colX > 0);
    W1 = Nilib::Matrixf::diag(colX, outdim, 1.0f) + Nilib::Matrixf::randn(colX, outdim, 0.0f, 0.001f);
    W2 = Nilib::Matrixf::diag(colX, outdim, 1.0f) + Nilib::Matrixf::randn(colX, outdim, 0.0f, 0.001f);
}

void GraphConvHigh::evaluate()
{
    A.evaluate();
    X.evaluate();
    W1.evaluate();
    W2.evaluate();
    ASSERT(A.value.cols() == X.value.rows(), "AX undefined:", A.value, 'x', X.value);
    ASSERT(X.value.cols() == W1.value.rows(), "XW1 undefined:", X.value, 'x', W1.value);
    ASSERT(X.value.cols() == W2.value.rows(), "XW2 undefined:", X.value, 'x', W2.value);
    this->value = alpha * A.value * X.value * W1.value + (1.0f - alpha) * X.value * W2.value;
}

void GraphConvHigh::learnables(Module::Weights &add)
{
    add.push_back(&W1);
    add.push_back(&W2);
}

void GraphConvHigh::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == A.value.rows());
    CORE_ASSERT(seed.cols() == W1.value.cols());
    A.derive(alpha * seed * transpose(X.value * W1.value));
    X.derive(alpha * transpose(A.value) * seed * transpose(W1.value) + (1.0f - alpha) * seed * transpose(W2.value));
    W1.derive(transpose(A.value * X.value) * seed * alpha);
    W2.derive(transpose(X.value) * seed * (1.0f - alpha));
}