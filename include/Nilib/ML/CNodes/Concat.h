#ifndef _CNODE_CONCAT_H
#define _CNODE_CONCAT_H

#include "Nilib/ML/CNodes/CNode.h"

namespace Nilib
{
    // Concatentation Gate.
    struct Cbind : public CNode
    {
        CNode *lhs = nullptr, *rhs = nullptr;

        Cbind(CNode *lhs, CNode *rhs);

        void evaluate() override;
        void derive(Matrixf const &seed) override;
    };

    // A, B -> [A, B]
    struct Rbind : public CNode
    {
        CNode *lhs = nullptr, *rhs = nullptr;

        Rbind(CNode *lhs, CNode *rhs);

        void evaluate() override;
        void derive(Matrixf const &seed) override;
    };
}
#endif