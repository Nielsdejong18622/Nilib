#ifndef _ML_MLP_H
#define _ML_MLP_H

#include "Nilib/ML/Model.h"
#include "Nilib/ML/CNodes/CNode.h"
#include "Nilib/ML/CNodes/Weight.h"
#include "Nilib/ML/CNodes/Plus.h"
#include "Nilib/ML/CNodes/Param.h"
#include "Nilib/ML/Cnodes/Layer/DenseLayer.h"

#include "Nilib/ML/Cnodes/Activation/Relu.h"
#include "Nilib/ML/Cnodes/Activation/Sigmoid.h"
#include "Nilib/ML/Cnodes/Activation/Tanh.h"

#include "Nilib/ML/Cnodes/Loss/MSE.h"
#include "Nilib/ML/Cnodes/Regularizer/L2.h"

namespace Nilib
{
    struct MLP : public Model
    {
        Input x;
        Target y;
        Param labda;
        Weight W1, W2;
        Weight b1, b2;

        DenseLayer H1, H3;
        Sigmoid H2;
        Sigmoid out;

        MSELoss MSE;
        L2 REG;
        Plus TOTLOS;

    public:
        // sigmoid(sigmoid(input * W1 + b1) * W2 + b2)
        MLP(size_t inputdim_n, size_t inputdim_m, size_t neurons1, size_t outputdim_m);
        MLP(MLP const &other);
    };
} // namespace Nilib

#endif