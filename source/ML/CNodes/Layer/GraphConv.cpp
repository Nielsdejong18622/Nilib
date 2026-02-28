
#include "Nilib/ML/CNodes/Layer/GraphConv.hpp"

#include "Nilib/Math/LinAlg.hpp"

using namespace Nilib;

GraphConv::GraphConv(CNode &A, CNode &X, size_t const colX, size_t const outdim) : d_A(A), d_X(X), d_W(colX, outdim)
{
    CORE_ASSERT(outdim > 0);
    CORE_ASSERT(colX > 0);
}

void GraphConv::evaluate()
{
    d_A.evaluate();
    d_X.evaluate();
    d_W.evaluate();
    ASSERT(d_A.value.cols() == d_X.value.rows(), "AX undefined:", d_A.value, 'x', d_X.value);
    ASSERT(d_X.value.cols() == d_W.value.rows(), "XW undefined:", d_X.value, 'x', d_W.value);
    this->value = d_A.value * d_X.value * d_W.value;
}

void GraphConv::learnables(Module::Weights &add)
{
    add.push_back(&d_W);
}

void GraphConv::derive(Nilib::Matrixf const &seed)
{
    CORE_ASSERT(seed.rows() == d_A.value.rows());
    CORE_ASSERT(seed.cols() == d_W.value.cols());

    CORE_ASSERT(d_A.value.rows() == seed.rows());
    CORE_ASSERT(d_A.value.cols() == d_X.value.rows());

    d_A.derive(seed * transpose(d_X.value * d_W.value));

    CORE_ASSERT(d_X.value.rows() == d_A.value.cols());
    CORE_ASSERT(d_X.value.cols() == d_W.value.rows());
    d_X.derive(transpose(d_A.value) * seed * transpose(d_W.value));

    CORE_ASSERT(d_W.value.rows() == d_X.value.cols());
    CORE_ASSERT(d_W.value.cols() == seed.cols());
    d_W.derive(transpose(d_A.value * d_X.value) * seed);
}