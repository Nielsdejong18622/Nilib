#ifndef _DENSE_H
#define _DENSE_H

#include "Nilib/ML/Layers/NeuralLayer.hpp"

// A fully connected neural layer.
class Dense : public NeuralLayer
{
protected:
    size_t const d_inputdim;
    size_t const d_outputdim;

    // Holds the current weights. 
    Nilib::Matrixf d_weights;
    // Holds the accumulated gradients.
    Nilib::Matrixf d_weight_grads;

    Nilib::Matrixf d_bias;
    Nilib::Matrixf d_bias_grads;
public:
    Dense(size_t const inputdim, size_t const outputdim);

    Nilib::Matrixf forward(Nilib::Matrixf const &X);
    Nilib::Matrixf backward(Nilib::Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return d_inputdim * d_outputdim + d_outputdim; };
};

#endif