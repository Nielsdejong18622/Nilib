#ifndef _OUTERDECODER_H
#define _OUTERDECODER_H

#include "Nilib/ML/Layers/NeuralLayer.hpp"

// A fully connected neural layer.
class OuterDecoder : public NeuralLayer
{
protected:

public:
    OuterDecoder();

    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return 0; };
};

#endif