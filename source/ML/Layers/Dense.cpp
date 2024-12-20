#include "ML/Layers/Dense.hpp"

Dense::Dense(size_t const inputdim, size_t const outputdim)
    : d_inputdim(inputdim), d_outputdim(outputdim), 
    d_weights(Matrixf::create_randn(inputdim, outputdim, 0.0, 1.0)), d_weight_grads(inputdim, outputdim),
    d_bias(Matrixf::create_randn(1, outputdim, 0.0, 1.0)), d_bias_grads(1, outputdim)
{
}

Matrixf Dense::forward(Matrixf const &X)
{
    CORE_ASSERT(X.rows() == 1)
    CORE_ASSERT(X.cols() == d_inputdim)
    d_input = X;

    auto output = X * d_weights + d_bias;

    CORE_ASSERT(!output.containsNA());
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
    
    CORE_ASSERT(!d_weight_grads.containsNA())
    // Return deriv to input. dL/dX
    return  error * transpose(d_weights);
}

void Dense::update(Optimizer &optim)
{
    // Gradient clipping. 
    d_weights -= optim.lr * Matrixf::apply(d_weight_grads, [](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
    d_bias -= optim.lr * Matrixf::apply(d_bias_grads, [](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
}

void Dense::zeroGrad()
{
    d_weight_grads.zeros();
    d_bias_grads.zeros();
}
void Dense::info() const
{
    Log::debug("DenseNeuralLayer ") << d_inputdim << 'x' << d_outputdim << ".\n";
}
void Dense::display() const
{
    Log::debug("DenseNeuralLayer ") << d_inputdim << 'x' << d_outputdim << ".\n";
    Log::debug("Weights ") << d_weights << " acc grad " << d_weight_grads << " Bias " << d_bias << " acc grad " << d_bias_grads << ".\n";
}
