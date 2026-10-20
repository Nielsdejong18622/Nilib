#include "Nilib/ML/CNodes/Layer/GraphConvHighWay.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GraphConvHigh::GraphConvHigh(CNode &A, CNode &X, size_t const colX, size_t const outdim)
    : d_A(A), d_X(X), d_W1(colX, outdim), d_W2(colX, outdim)
{
    CORE_ASSERT(outdim > 0);
    CORE_ASSERT(colX > 0);
    d_W1.value = Nilib::Matrixf::diag(colX, outdim, 1.0f) + Nilib::Matrixf::randn(colX, outdim, 0.0f, 0.1f);
    d_W2.value = Nilib::Matrixf::diag(colX, outdim, 1.0f) + Nilib::Matrixf::randn(colX, outdim, 0.0f, 0.1f);
}

void GraphConvHigh::evaluate()
{
    d_A.evaluate();
    d_X.evaluate();
    d_W1.evaluate();
    d_W2.evaluate();
    CORE_ASSERT(d_X.value.cols() == d_W1.value.rows());
    CORE_ASSERT(d_X.value.cols() == d_W2.value.rows());
    CORE_ASSERT(d_A.value.rows() == d_X.value.rows());
    CORE_ASSERT(d_W1.value.cols() == d_W2.value.cols());
    this->value = d_alpha * d_A.value * d_X.value * d_W1.value + (1.0f - d_alpha) * d_X.value * d_W2.value;
}

void GraphConvHigh::learnables(Module::Weights &add)
{
    add.push_back(&d_W1);
    add.push_back(&d_W2);
}

void GraphConvHigh::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == d_A.value.rows());
    CORE_ASSERT(seed.cols() == d_W1.value.cols());

    CORE_ASSERT(d_A.value.rows() == seed.rows());
    CORE_ASSERT(d_A.value.cols() == d_X.value.rows());

    d_A.derive(d_alpha * seed * transpose(d_X.value * d_W1.value));

    CORE_ASSERT(d_X.value.rows() == d_A.value.cols());
    CORE_ASSERT(d_X.value.cols() == d_W1.value.rows());
    d_X.derive(d_alpha * transpose(d_A.value) * seed * transpose(d_W1.value) + (1.0f - d_alpha) * seed * transpose(d_W2.value));

    CORE_ASSERT(d_W1.value.rows() == d_X.value.cols());
    CORE_ASSERT(d_W1.value.cols() == seed.cols());
    d_W1.derive(transpose(d_A.value * d_X.value) * seed * d_alpha);

    CORE_ASSERT(d_W2.value.rows() == d_X.value.cols());
    CORE_ASSERT(d_W2.value.cols() == seed.cols());
    d_W2.derive(transpose(d_X.value) * seed * (1.0f - d_alpha));
}