#ifndef _L2_REGULAZIZER
#define _L2_REGULAZIZER

#include "Nilib/ML/CNodes/Regularizer/Regularizer.h"

namespace Nilib
{
    struct L2: public Regularizer
    {
        L2(std::initializer_list<CNode *> const &weights, Param *labda)
        : Regularizer(weights, labda) {}

        float penalty(float t, Matrixf const &w) override { return 0.5 * t * t; }
        float penalty_deriv(float t, Matrixf const &w) override { return t; }
    };
}

#endif