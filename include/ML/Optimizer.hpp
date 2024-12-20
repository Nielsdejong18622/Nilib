#ifndef _OPTIMIZER_H
#define _OPTIMIZER_H

// In charge of how and when to update the weights. 
struct Optimizer
{
    struct OptimizerParams
    {

    } params;

    unsigned long long int iteration = 0;
    float lr = 0.01;
};

#endif