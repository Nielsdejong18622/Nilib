#ifndef _INEURAL_LAYER_H
#define _INEURAL_LAYER_H

#include "Nilib/Math/LinAlg.hpp"
#include "Nilib/ML/Optimizer.hpp"
#include <vector>

class NeuralNet;

// A neural layer is just a node in the computational graph. 
// A function, if you will. 
class NeuralLayer
{
protected:
    NeuralNet *d_model = nullptr;

    // Feedforward input cache. 
    Nilib::Matrixf d_input;
public:
    friend class NeuralNet;
    //NeuralLayer();
    virtual ~NeuralLayer() {};

    virtual Nilib::Matrixf forward(Nilib::Matrixf const &X) = 0; 
    virtual Nilib::Matrixf backward(Nilib::Matrixf const &output_error) = 0;
    virtual void info() const = 0;
    
    virtual void update(Optimizer &optim) = 0;
    virtual void zeroGrad() = 0;
    virtual void display() const = 0;

    virtual size_t learnables() const { return 0; };
};


#endif