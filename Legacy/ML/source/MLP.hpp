#ifndef _MLP_H
#define _MLP_H

#include "Nilib/Math/LinAlg.hpp"


template<size_t inputdimm, size_t hidden, size_t outputdim, size_t parallel = 1>
struct MLP
{
    using Input = Nilib::Mat<parallel, inputdimm>;
    using Weights = Nilib::Mat<inputdimm, hidden>;
    using Bias = Nilib::Mat<parallel, hidden>;
    using Output = Nilib::Mat<parallel, hidden>;

    Weights weight;
    Bias bias;
    Output value;

    MLP()
    {
        // Randomly set the weights. 
        weight.setrandn(0, 1);
        bias.setrandn(0,1);
    }

    Output forward(Input *input) 
    {
        value = (*input) * weight + bias;
        return value;
    }

    // Input backward(Output const &backward)
    // {
    //     return Nilib::transpose()
    // }


};


#endif