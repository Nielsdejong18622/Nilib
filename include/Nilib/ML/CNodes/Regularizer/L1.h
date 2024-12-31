#ifndef _L1_REGULAZIZER
#define _L1_REGULAZIZER

#include "Nilib/ML/CNodes/Regularizer/Regularizer.h"

namespace Nilib
{
    struct L1: public Regularizer
    {
        L1(std::initializer_list<CNode *> const &weights, Param *labda)
        : Regularizer(weights, labda) {}

        float penalty(float t, Matrixf const &w) override { return std::abs(t); }
        float penalty_deriv(float t, Matrixf const &w) override { return (t == 0.0)?0.0:((t > 0.0)?1.0:-1.0); }
    };
}

#endif