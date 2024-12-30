#include "Nilib/ML/Layers/BatchNorm.hpp"

using namespace Nilib;

BatchNorm::BatchNorm(size_t const inputdim, float const momentum)
    : d_inputdim(inputdim),  
    d_momentum(momentum),
    d_weights(Matrixf::randn(inputdim, 2, 0.0, 1.0)), d_weight_grads(inputdim, 2)
{
}

Matrixf BatchNorm::forward(Matrixf const &X)
{
    d_input = X;
    
    // Calculate the (running) mean and variance. 
    //d_running_mean = d_momentum * d_running_mean + (1 - d_momentum) * colMeans(X);
    //d_running_var = d_momentum * d_running_var + (1 - d_momentum) * colVars(X);

    // Normalize the input X.
    //Matrixf normX = (X - d_running_mean) / d_running_var;

    // Multiply by scale and shift by shift.
    return X; // * d_weights(0,0) + d_weights(0, 1);
}

// We receive an error matrix.
Matrixf BatchNorm::backward(Matrixf const &error)
{

    return error;
}

void BatchNorm::update(Optimizer &optim)
{
    // Gradient clipping. 
    d_weight_grads.apply([](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
    d_weights -= optim.lr * d_weight_grads;
}

void BatchNorm::zeroGrad()
{
    d_weight_grads.zero();
}
void BatchNorm::info() const
{
    LOG_DEBUG() << "BatchNorm " << d_inputdim << ".\n";
}
void BatchNorm::display() const
{
    LOG_DEBUG() << "BatchNorm " << d_inputdim << ".\n";
    LOG_DEBUG() << "Weights " << d_weights << " acc grad " << d_weight_grads << ".\n";
}
