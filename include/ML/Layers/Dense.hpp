#ifndef _DENSE_H
#define _DENSE_H

#include "ML/Layers/NeuralLayer.hpp"

// A fully connected neural layer.
class Dense : public NeuralLayer
{
protected:
    size_t const d_inputdim;
    size_t const d_outputdim;

    // Holds the current weights. 
    Matrixf d_weights;
    // Holds the accumulated gradients.
    Matrixf d_weight_grads;

    Matrixf d_bias;
    Matrixf d_bias_grads;
public:
    Dense(size_t const inputdim, size_t const outputdim);

    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return d_inputdim * d_outputdim + d_outputdim; };
};

#endif