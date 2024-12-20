#ifndef _GRAPHCONV_H
#define _GRAPHCONV_H

#include "ML/Layers/NeuralLayer.hpp"

// A fully connected neural layer.
class GraphConv : public NeuralLayer
{
protected:
    size_t const d_inputdim;
    size_t const d_outputdim;

    // Holds the current weights. 
    Matrixf d_weights;
    // Holds the accumulated gradients.
    Matrixf d_weight_grads;
public:
    GraphConv(size_t const inputdim, size_t const outputdim);

    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return d_inputdim * d_outputdim; };
};

#endif