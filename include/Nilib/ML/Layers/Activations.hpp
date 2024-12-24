#ifndef _ACTIVATIONS_H
#define _ACTIVATIONS_H

#include "Nilib/ML/Layers/NeuralLayer.hpp"
#include "Nilib/ML/Optimizer.hpp"

class Linear : public NeuralLayer
{
public:
    Nilib::Matrixf forward(Nilib::Matrixf const &X);
    Nilib::Matrixf backward(Nilib::Matrixf const &X);
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
    Nilib::Matrixf forward(Nilib::Matrixf const &X);
    Nilib::Matrixf backward(Nilib::Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
};

class Sigmoid : public NeuralLayer
{
public:
    Nilib::Matrixf forward(Nilib::Matrixf const &X);
    Nilib::Matrixf backward(Nilib::Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
};

#endif