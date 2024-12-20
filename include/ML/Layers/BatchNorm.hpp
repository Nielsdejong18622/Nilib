#ifndef _BATCH_NORM_HPP
#define _BATCH_NORM_HPP

#include "ML/Layers/NeuralLayer.hpp"

// BatchNorm layer that uses running averages. 
class BatchNorm : public NeuralLayer
{
protected:
    size_t const d_inputdim;

    // Holds the running averages/stds over the batch. 
    // d_running(0,0) is the mean and d_running(0, 1) the variance.
    Matrixf d_running_mean;
    Matrixf d_running_var;
    float d_momentum = 0.0; // Governs the update of running mean and variance. 
    
    // d_weights(0,0) is the scale and d_weights(0,1) the shift.
    Matrixf d_weights;

    // Holds the accumulated gradients.
    Matrixf d_weight_grads;

public:
    BatchNorm(size_t const inputdim, float const momentum = 0.1f);

    Matrixf forward(Matrixf const &X);
    Matrixf backward(Matrixf const &X);
    void update(Optimizer &optim);
    void zeroGrad();
    void info() const;

    void display() const;
    size_t learnables() const { return 2; };
};

#endif