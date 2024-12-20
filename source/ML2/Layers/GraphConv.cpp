#include "ML/Layers/GraphConv.hpp"
#include "ML/NeuralNet.hpp"

GraphConv::GraphConv(size_t const inputdim, size_t const outputdim)
    : d_inputdim(inputdim), d_outputdim(outputdim), 
    d_weights(Matrixf::create_randn(inputdim, outputdim, 0.0, 1.0)), d_weight_grads(inputdim, outputdim)
{
}

Matrixf GraphConv::forward(Matrixf const &X)
{
    // Obtain the adjacency matrix. 
    CORE_ASSERT(d_model != nullptr)
    Matrixf const &A = d_model->adjacency();
    CORE_ASSERT(A.rows() == X.rows())
    CORE_ASSERT(X.cols() == d_weights.rows());
    d_input = X;
    Matrixf const output = A * X * d_weights;
    CORE_ASSERT(output.rows() == A.rows())
    CORE_ASSERT(output.cols() == d_outputdim)
    //CORE_ASSERT(!output.containsNA());
    return output;
}
// We receive an error matrix.
Matrixf GraphConv::backward(Matrixf const &error)
{
    // Obtain the adjacency matrix. 
    Matrixf const &A = d_model->adjacency();
    CORE_ASSERT(A.rows() == error.rows())
    CORE_ASSERT(A.cols() == d_input.rows())

    // Find out which weights are responsible.
    // dL/dW    
    d_weight_grads += transpose(A * d_input) * error;
    
    
    CORE_ASSERT(!d_weight_grads.containsNA())
    // Return deriv to input. dL/dX
    CORE_ASSERT(A.rows() == A.cols())
    CORE_ASSERT(error.cols() == d_weights.cols());

    Matrixf const output = transpose(A) * error * transpose(d_weights);
    //CORE_ASSERT(!output.containsNA());
    return output;
}

void GraphConv::update(Optimizer &optim)
{
    // Gradient clipping. 
    d_weights -= optim.lr * Matrixf::apply(d_weight_grads, [](float const t) { return std::min(std::max(t, -1.0f), 1.0f);});
}

void GraphConv::zeroGrad()
{
    d_weight_grads.zeros();
}
void GraphConv::info() const
{
    Log::debug("GraphConvLayer ") << d_inputdim << 'x' << d_outputdim << ".\n";
}
void GraphConv::display() const
{
    Log::debug("GraphConvLayer ") << d_inputdim << 'x' << d_outputdim << ".\n";
    Log::debug("Weights ") << d_weights << " acc grad " << d_weight_grads << ".\n";
}
