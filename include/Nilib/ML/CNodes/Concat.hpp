#ifndef _CNODE_CONCAT_H
#define _CNODE_CONCAT_H

#include "Nilib/ML/CNodes/CNode.hpp"

namespace Nilib
{
    // Concatentation Gate.
    class Cbind : public CNode
    {
    public:
        Cbind(CNode &lhs, CNode &rhs);

        void evaluate() override;
        void derive(Matrixf const &seed) override;

    private:
        CNode &lhs, &rhs;
    };

    // A, B -> [A, B]
    class Rbind : public CNode
    {
    public:
        Rbind(CNode &lhs, CNode &rhs);

        void evaluate() override;
        void derive(Matrixf const &seed) override;

    private:
        CNode &lhs, &rhs;
    };
}
#endif