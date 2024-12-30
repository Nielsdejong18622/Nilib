#ifndef _CNODE_H
#define _CNODE_H

#include "Nilib/Math/Matrix.hpp"



// A computational node in a computational graph.
struct CNode
{
    Nilib::Matrixf value;

    //virtual void evaluate() = 0;
    //virtual void derive() = 0;
};



struct MLP
{

    Nilib::Matrixf feedforward(Nilib::Matrixf &x)
    {
    //     Input X(x);
    //     Weight W;
    //     Weight B;
    //     CNode X1 = W * X;
    //     CNode X2 = X1 + B;
    //     CNode X3 = Sigma(X2);

    //     return X3.cache();
    }
};

#endif