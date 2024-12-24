#include "Nilib/ML/Layers/Dense.hpp"

using namespace Nilib;

Dense::Dense(size_t const inputdim, size_t const outputdim)
    : d_inputdim(inputdim), d_outputdim(outputdim), 
    d_weights(Matrixf::randn(inputdim, outputdim, 0.0, 1.0)), d_weight_grads(inputdim, outputdim),
    d_bias(Matrixf::randn(1, outputdim, 0.0, 1.0)), d_bias_grads(1, outputdim)
{
}

Matrixf Dense::forward(Matrixf const &X)
{
    CORE_ASSERT(X.rows() == 1)
    CORE_ASSERT(X.cols() == d_inputdim)
    d_input = X;

    auto output = X * d_weights + d_bias;

    return output;
}
// We receive an error matrix.
Matrixf Dense::backward(Matrixf const &error)
{
    CORE_ASSERT(error.rows() == d_input.rows())
    CORE_ASSERT(d_input.cols() == d_weight_grads.rows() && error.cols() == d_weight_grads.cols())
    // Find out which weights are responsible.
    // dL/dW
    d_weight_grads += transpose(d_input) * error;
    
    // dL/dbias
    d_bias_grads += error;
    
    // Return deriv to input. dL/dX
    return  error * transpose(d_weights);
}

void Dense::update(Optimizer &optim)
{
    // Gradient clipping. 
    d_weights.apply([](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
    d_weights -= optim.lr * d_weight_grads;
    d_bias_grads.apply([](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
    d_bias -= optim.lr * d_bias_grads;
}

void Dense::zeroGrad()
{
    d_weight_grads.zero();
    d_bias_grads.zero();
}
void Dense::info() const
{
    LOG_DEBUG() << "DenseNeuralLayer " << d_inputdim << 'x' << d_outputdim << ".\n";
}
void Dense::display() const
{
    LOG_DEBUG() << "DenseNeuralLayer " << d_inputdim << 'x' << d_outputdim << ".\n";
    LOG_DEBUG() << "Weights " << d_weights << " acc grad " << d_weight_grads << " Bias " << d_bias << " acc grad " << d_bias_grads << ".\n";
}
