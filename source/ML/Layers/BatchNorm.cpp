#include "ML/Layers/BatchNorm.hpp"

BatchNorm::BatchNorm(size_t const inputdim, float const momentum)
    : d_inputdim(inputdim),  
    d_momentum(momentum),
    d_weights(Matrixf::create_randn(inputdim, 2, 0.0, 1.0)), d_weight_grads(inputdim, 2)
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
    return X * d_weights(0,0) + d_weights(0, 1);
}

// We receive an error matrix.
Matrixf BatchNorm::backward(Matrixf const &error)
{

    return error;
}

void BatchNorm::update(Optimizer &optim)
{
    // Gradient clipping. 
    d_weights -= optim.lr * Matrixf::apply(d_weight_grads, [](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
}

void BatchNorm::zeroGrad()
{
    d_weight_grads.zeros();
}
void BatchNorm::info() const
{
    Log::debug("BatchNorm ") << d_inputdim << ".\n";
}
void BatchNorm::display() const
{
    Log::debug("BatchNorm ") << d_inputdim << ".\n";
    Log::debug("Weights ") << d_weights << " acc grad " << d_weight_grads << ".\n";
}
