#ifndef _NORMALIZATION_LAYER
#define _NORMALIZATION_LAYER

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{

    struct Norm : public CNode
    {
        CNode *x = nullptr;

        Nilib::Matrixf mean;
        Norm(CNode *X, size_t xrow, size_t xcol);

        void evaluate() override;
        void derive(Nilib::Matrixf const &seed) override;
    };
} // namespace Nilib

#endif