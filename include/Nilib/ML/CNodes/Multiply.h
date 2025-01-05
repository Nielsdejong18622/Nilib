
#ifndef _CNODE_MULTIPLY_H
#define _CNODE_MULTIPLY_H

#include "Nilib/ML/CNodes/CNode.h"
namespace Nilib
{

    // Multiply Gate.
    struct Multiply : public CNode
    {
        CNode *lhs = nullptr, *rhs = nullptr;
        Multiply(CNode *lhs, CNode *rhs) : lhs(lhs), rhs(rhs) {}

        void evaluate()
        {
            lhs->evaluate();
            rhs->evaluate();
            this->value = lhs->value * rhs->value;
        }

        void derive(Matrixf const &seed)
        {
            lhs->derive(seed * transpose(rhs->value));
            rhs->derive(transpose(lhs->value) * seed);
        }
    };
}
#endif