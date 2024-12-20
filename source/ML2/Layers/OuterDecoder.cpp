#include "ML/Layers/OuterDecoder.hpp"
#include "ML/NeuralNet.hpp"

OuterDecoder::OuterDecoder()
{
}

Matrixf OuterDecoder::forward(Matrixf const &X)
{
    // Obtain the adjacency matrix. 
    d_input = X;
    //CORE_ASSERT(!output.containsNA());
    Matrixf tX = transpose(X);
    //d::cerr << "OuterDecoder transpose: " << tX << '\n';
    Matrixf output = X * tX;
    //std::cerr << "OuterDecoder output: " << output << '\n';
    output /= X.rows();

    return output;
}
// We receive an error matrix.
Matrixf OuterDecoder::backward(Matrixf const &error)
{
    //ASSERT(false, "Backwards for outerproduct decoder not yet implemented!");

    return (1.0 / d_input.rows()) * ((transpose(d_input) * error) + error * d_input);
}

void OuterDecoder::update(Optimizer &optim)
{
}

void OuterDecoder::zeroGrad()
{
    
}
void OuterDecoder::info() const
{
    Log::debug("OuterDecoderLayer.\n");
}
void OuterDecoder::display() const
{
    info();
}
