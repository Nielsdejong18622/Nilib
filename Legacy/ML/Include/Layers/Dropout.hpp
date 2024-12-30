#ifndef _DROPOUT_H
#define _DROPOUT_H

#include "Nilib/ML/Layers/NeuralLayer.hpp"

// A fully connected neural layer.
class Dropout : public NeuralLayer
{
protected:
    // The chance that individual inputs will be disabled. 
    float const d_rate;
    // Holds the last applied mask weights. 
    Nilib::Matrixf d_mask;
    

public:
    Dropout(float const rate);

    Nilib::Matrixf forward(Nilib::Matrixf const &X);
    Nilib::Matrixf backward(Nilib::Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return 0; };
};

#endif