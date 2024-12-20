#ifndef _GRAPH_POOL_H
#define _GRAPH_POOL_H

#include "ML/Layers/NeuralLayer.hpp"

class GraphPool : public NeuralLayer
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