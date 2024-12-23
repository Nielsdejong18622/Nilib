#include "Nilib/ML/Layers/Dropout.hpp"

Dropout::Dropout(float const rate)
: d_rate(rate)
{
    
}

Matrixf Dropout::forward(Matrixf const &X)
{    
    float const rate = d_rate;
    d_mask = Matrixf::create_rand(X.rows(), X.cols());
    d_mask.apply([rate](float const t) {return (t > rate) ? 1.0 / (1.0 - rate) : 0.0;});;

    return hadamar(d_mask, X);
}
// We receive an error matrix.
Matrixf Dropout::backward(Matrixf const &error)
{
    // Return deriv to input. dL/dX
    return hadamar(d_mask, error);
}

void Dropout::update(Optimizer &optim)
{
}

void Dropout::zeroGrad()
{
}
void Dropout::info() const
{
    LOG_DEBUG() << "DropoutNeuralLayer rate " << d_rate << " mask " << d_mask << ".\n";
}
void Dropout::display() const
{
    LOG_DEBUG() << "DropoutNeuralLayer rate " << d_rate << ".\n";
}
