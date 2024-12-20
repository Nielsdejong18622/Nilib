#ifndef _ACTIVATIONS_H
#define _ACTIVATIONS_H

#include "ML/Layers/NeuralLayer.hpp"
#include "ML/Optimizer.hpp"

class Linear : public NeuralLayer
{
public:
    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
};

class Relu : public NeuralLayer
{
    float const d_leakyalpha = 0.0;
public:
    Relu();
    Relu(float const leakyalpha);
    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
};

class Sigmoid : public NeuralLayer
{
public:
    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
};

#endif