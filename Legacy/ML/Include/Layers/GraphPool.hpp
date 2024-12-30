#ifndef _GRAPH_POOL_H
#define _GRAPH_POOL_H

#include "Nilib/ML/Layers/NeuralLayer.hpp"

class GraphPool : public NeuralLayer
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