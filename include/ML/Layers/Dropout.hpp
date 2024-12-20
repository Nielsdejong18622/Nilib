#ifndef _DROPOUT_H
#define _DROPOUT_H

#include "ML/Layers/NeuralLayer.hpp"

// A fully connected neural layer.
class Dropout : public NeuralLayer
{
protected:
    // The chance that individual inputs will be disabled. 
    float const d_rate;
    // Holds the last applied mask weights. 
    Matrixf d_mask;
    

public:
    Dropout(float const rate);

    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return 0; };
};

#endif