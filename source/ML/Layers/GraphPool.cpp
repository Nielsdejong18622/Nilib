#include "Nilib/ML/Layers/GraphPool.hpp"

Matrixf GraphPool::forward(Matrixf const &X)
{
    d_input = X;
    Matrixf ones = Matrixf::create_ones(1, X.rows());
    return (1.0f / X.rows()) * ones * X;
}

Matrixf GraphPool::backward(Matrixf const &error)
{
    Matrixf res = Matrixf(d_input.rows(), 1);
    res.fill((float) 1.0f / d_input.rows());
    CORE_ASSERT(res.cols() == error.rows());
    return res * error;
}
void GraphPool::update(Optimizer &optim) {}
void GraphPool::zeroGrad() {}
void GraphPool::info() const
{
    Log::info("GraphNodePoolMean layer ") << this << ".\n";
}
void GraphPool::display() const
{
    Log::info("GraphNodePoolMean layer ") << this << ".\n";
}
