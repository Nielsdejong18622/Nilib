#include "Nilib/ML/Layers/Activations.hpp"

using namespace Nilib;

Relu::Relu()
: d_leakyalpha(0)
{

}

Relu::Relu(float const leakyalpha = 0.05)
: d_leakyalpha(leakyalpha)
{

}
Matrixf Relu::forward(Matrixf const &X)
{
    float la = d_leakyalpha;
    d_input = X;
    Matrixf res = X;
    res.apply([la](float const t){ return (t > 0) ? t : la * t; });
    return res;
}
Matrixf Relu::backward(Matrixf const &error)
{
    float la = d_leakyalpha;
    CORE_ASSERT(error.rows() == d_input.rows())
    CORE_ASSERT(error.cols() == d_input.cols())
    Matrixf res = d_input;
    res.apply([la](float const t){ return (t > 0) ? 1 : la; } );
    return hadamar(res, error);
}

void Relu::display() const
{
    LOG_DEBUG() << "LeakyRelu NeuralLayer leaky " << d_leakyalpha << ".\n";
}
void Relu::info() const
{
    LOG_DEBUG() << "LeakyRelu NeuralLayer.\n";
}
void Relu::update(Optimizer &)
{
}
void Relu::zeroGrad()
{
}

Matrixf Sigmoid::forward(Matrixf const &X)
{
    d_input = X;
    Matrixf res = X;
    res.apply([](float const t) {return (1 / ( 1 + std::exp(-t)));});
    return res;
}
Matrixf Sigmoid::backward(Matrixf const &error)
{
    CORE_ASSERT(error.rows() == d_input.rows())
    CORE_ASSERT(error.cols() == d_input.cols())
    Matrixf res = d_input;
    res.apply([](float const t) {return (1 / ( 1 + std::exp(-t))) * (1 - (1 / ( 1 + std::exp(-t))));});
    return hadamar(res, error);
}
void Sigmoid::display() const
{
    LOG_DEBUG() << "Sigmoid NeuralLayer.\n";
}
void Sigmoid::info() const
{
    LOG_DEBUG() << "Sigmoid NeuralLayer.\n";
}
void Sigmoid::update(Optimizer &)
{
}
void Sigmoid::zeroGrad()
{
}